


#include "Player/PstPlayerController.h"
#include "Engine/LocalPlayer.h"
#include "Input/PstInputComponent.h"
#include "Input/PstInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayAbility/PstAbilitySystemComponent.h"
#include "Player/PstPlayerState.h"
#include "PstGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "UI/PstUserWidget_DamageDisplay.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstPlayerController)

APstPlayerController::APstPlayerController()
{
	bAttachToPawn = true;

}

void APstPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ViewPitchMin = -80.0f;
		PlayerCameraManager->ViewPitchMax = 60.0f;
	}
}

void APstPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(GetLocalPlayer());
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	for (const UInputMappingContext* InputMappingContext : InputMappings)
	{
		FModifyContextOptions Options = {};
		Options.bIgnoreAllPressedKeysUntilRelease = false;
		Subsystem->AddMappingContext(InputMappingContext, 0, Options);
	}

	if (UPstInputComponent* PstInputComponent = Cast<UPstInputComponent>(InputComponent))
	{
		for (const UPstInputConfig* InputConfig : InputConfigs)
		{

		}
	}
}

void APstPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetGenericTeamId(1);
}

void APstPlayerController::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void APstPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UPstAbilitySystemComponent* PstAbilitySystemComponent = GetPstAbilitySystemComponent())
	{
		PstAbilitySystemComponent->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

UPstAbilitySystemComponent* APstPlayerController::GetPstAbilitySystemComponent() const
{
	const APstPlayerState* PstPlayerState = GetPlayerState<APstPlayerState>();
	return (PstPlayerState ? PstPlayerState->GetAbilitySystemComponent<UPstAbilitySystemComponent>() : nullptr);
}

void APstPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamId)
{
	if (TeamId != NewTeamId)
	{
		TeamId = NewTeamId;
	}
}

FGenericTeamId APstPlayerController::GetGenericTeamId() const
{
	return TeamId;
}

void APstPlayerController::PlayHitSound2D_Implementation(bool bCriticalHit)
{
	if (bCriticalHit)
	{
		if (CriticalHitSound)
		{
			UGameplayStatics::PlaySound2D(this, CriticalHitSound);
		}
	}
	else
	{
		if (NormalHitSound)
		{
			UGameplayStatics::PlaySound2D(this, NormalHitSound);
		}
	}
}

void APstPlayerController::ShowDamageDisplayWidget_Implementation(AActor* TargetActor, float DamageAmount, FSlateColor DamageColor, FVector TargetLocation)
{
	if (!DamageDisplayWidgetClass)
	{
		return;
	}

	if (TargetActor == nullptr)
	{
		return;
	}

	UWidgetComponent* DamageDisplayWidgetComponent = NewObject<UWidgetComponent>(TargetActor);
	UPstUserWidget_DamageDisplay* DamageDisplayWidget = CreateWidget<UPstUserWidget_DamageDisplay>(this, DamageDisplayWidgetClass);
	if (DamageDisplayWidgetComponent && DamageDisplayWidget)
	{
		DamageDisplayWidgetComponent->RegisterComponent();
		DamageDisplayWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DamageDisplayWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		DamageDisplayWidgetComponent->SetWidget(DamageDisplayWidget);
		DamageDisplayWidgetComponent->SetDrawAtDesiredSize(true);
		DamageDisplayWidgetComponent->SetIsReplicated(true);
		DamageDisplayWidget->SetDamageAmount(DamageAmount);
		DamageDisplayWidget->SetDamageColor(DamageColor);

		if (TargetLocation.IsNearlyZero())
		{
			DamageDisplayWidgetComponent->SetWorldLocation(TargetActor->GetActorLocation());
		}
		else
		{
			DamageDisplayWidgetComponent->SetWorldLocation(TargetLocation);
		}

		float DistanceToTarget = (GetPawn()->GetActorLocation() - TargetActor->GetActorLocation()).Size();
		float DistanceMultiplier = (DistanceToTarget > 1500.0f) ? DistanceToTarget * 0.0015f : DistanceToTarget * 0.0025f;
		FVector DamageWidgetLocationOffset = DistanceMultiplier * 30.0f * FMath::RandRange(1.0f, 1.25f) * GetPawn()->GetActorRightVector() + 15.0f * FMath::RandRange(1.0f, 1.5f) * GetPawn()->GetActorUpVector();
		if (TargetActor == GetPawn())
		{
			DamageWidgetLocationOffset.Z += 40.0f;
		}

		DamageDisplayWidgetComponent->SetWorldLocation(DamageDisplayWidgetComponent->GetComponentLocation() + DamageWidgetLocationOffset);
	}
}