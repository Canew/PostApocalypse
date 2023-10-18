


#include "AI/PstAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstAIController)

APstAIController::APstAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	bAttachToPawn = true;
	bWantsPlayerState = true;

	// Setup the perception component
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception")));

	SightSenseConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightSenseConfig"));
	SightSenseConfig->SightRadius = 2000.0f;
	SightSenseConfig->LoseSightRadius = 2000.0f;
	SightSenseConfig->SetMaxAge(8.0f);
	SightSenseConfig->PeripheralVisionAngleDegrees = 180.0f;
	GetPerceptionComponent()->ConfigureSense(*SightSenseConfig);

	DamageSenseConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageSenseConfig"));
	DamageSenseConfig->SetMaxAge(8.0f);
	GetPerceptionComponent()->ConfigureSense(*DamageSenseConfig);

	HearingSenseConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingSenseConfig"));
	HearingSenseConfig->HearingRange = 350.0f;
	HearingSenseConfig->SetMaxAge(8.0f);
	GetPerceptionComponent()->ConfigureSense(*HearingSenseConfig);

	GetPerceptionComponent()->SetDominantSense(SightSenseConfig->GetSenseImplementation());

	TargetActorKeyName = FName(TEXT("TargetActor"));
	HomeLocationKeyName = FName(TEXT("HomeLocation"));

	ControlRotationInterpSpeed = 30.0f;
}

void APstAIController::BeginPlay()
{
	Super::BeginPlay();

	RunAI();

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &APstAIController::OnTargetPerceptionUpdated);

	// Generic team
	SetGenericTeamId(2);
	UAIPerceptionSystem::GetCurrent(GetWorld())->UpdateListener(*GetAIPerceptionComponent());

	InitializeHomeLocation();
}

void APstAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	Super::UpdateControlRotation(DeltaTime, bUpdatePawn);

	return;

	// Smooth and change the pawn rotation
	if (bUpdatePawn)
	{
		// Get pawn
		APawn* const MyPawn = GetPawn();
		// Get Pawn current rotation
		const FRotator CurrentPawnRotation = MyPawn->GetActorRotation();

		FRotator FocusRotator;
		if (AActor* FocusActor = GetFocusActor())
		{
			FocusRotator = (FocusActor->GetActorLocation() - MyPawn->GetActorLocation()).Rotation();

			// Calculate smoothed rotation
			FRotator SmoothTargetRotation = UKismetMathLibrary::RInterpTo_Constant(MyPawn->GetActorRotation(), FocusRotator, DeltaTime, ControlRotationInterpSpeed);
			// Check if we need to change
			if (CurrentPawnRotation.Equals(SmoothTargetRotation, 1e-3f) == false)
			{
				// Change rotation using the Smooth Target Rotation
				MyPawn->FaceRotation(SmoothTargetRotation, DeltaTime);
			}
		}
	}
}

void APstAIController::RunAI()
{
	if (BlackboardData && BehaviorTree)
	{
		UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
		if (UseBlackboard(BlackboardData, BlackboardComponent))
		{
			RunBehaviorTree(BehaviorTree);
		}
	}
}

void APstAIController::RestartAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->RestartTree();
	}
}

void APstAIController::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree();
	}
}

ETeamAttitude::Type APstAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other))
	{
		if (OtherTeamAgent->GetGenericTeamId() == FGenericTeamId::NoTeam)
		{
			return ETeamAttitude::Neutral;
		}
		else
		{
			return FGenericTeamId::GetAttitude(GetGenericTeamId(), OtherTeamAgent->GetGenericTeamId());
		}
	}

	return ETeamAttitude::Neutral;
}

void APstAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (GetPawn() == nullptr)
	{
		return;
	}

	if (Stimulus.WasSuccessfullySensed())
	{
		TargetClosestHostileActor();
	}
	else
	{
		TargetClosestHostileActor();

		float TargetExpirationTime;
		if (SightSenseConfig)
		{
			TargetExpirationTime = SightSenseConfig->GetMaxAge();
		}
		else
		{
			TargetExpirationTime = 5.0f;
		}

		GetWorldTimerManager().SetTimer(TargetExpirationTimerHandle, this, &APstAIController::ExpireTarget, TargetExpirationTime, false);
	}
}

void APstAIController::InitializeHomeLocation()
{
	if (Blackboard)
	{
		if (APawn* MyPawn = GetPawn())
		{
			Blackboard->SetValueAsVector(HomeLocationKeyName, MyPawn->GetActorLocation());
		}
	}
}

void APstAIController::TargetClosestHostileActor()
{
	AActor* ClosestActor = nullptr;
	TArray<AActor*> PerceivedHostileActors;
	GetAIPerceptionComponent()->GetPerceivedHostileActors(PerceivedHostileActors);
	for (AActor* PerceivedHostileActor : PerceivedHostileActors)
	{
		UPrimitiveComponent* RootPrimitiveComponent = Cast<UPrimitiveComponent>(PerceivedHostileActor->GetRootComponent());
		if (!RootPrimitiveComponent || RootPrimitiveComponent->GetCollisionProfileName() == TEXT("NoCollision"))
		{
			continue;
		}

		UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PerceivedHostileActor);
		if (!AbilitySystemComponent || AbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Character.State.Cloaking"))))
		{
			continue;
		}

		if (!ClosestActor)
		{
			ClosestActor = PerceivedHostileActor;
			continue;
		}

		if (GetPawn()->GetDistanceTo(ClosestActor) > GetPawn()->GetDistanceTo(PerceivedHostileActor))
		{
			ClosestActor = PerceivedHostileActor;
		}
	}

	if (Blackboard)
	{
		Blackboard->SetValueAsObject(TargetActorKeyName, ClosestActor);
	}
}

void APstAIController::ExpireTarget()
{
	TargetClosestHostileActor();
}
