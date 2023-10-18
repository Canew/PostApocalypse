


#include "GameplayAbility/PstGameplayAbility_AimDownSights.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/PstCharacter.h"
#include "Character/PstCharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstGameplayAbility_AimDownSights)

bool UPstGameplayAbility_AimDownSights::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags))
	{
		return false;
	}

	return true;
}

void UPstGameplayAbility_AimDownSights::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);

		return;
	}

	UAbilityTask_WaitMovementModeChange* WaitMovementModeChange = UAbilityTask_WaitMovementModeChange::CreateWaitMovementModeChange(this, EMovementMode::MOVE_Falling);
	if (WaitMovementModeChange)
	{
		WaitMovementModeChange->OnChange.AddDynamic(this, &ThisClass::HandleMovementModeChange);
		WaitMovementModeChange->ReadyForActivation();
	}

	StartAimDownSights();

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor)
	{
		if (APstCharacter* AvatarCharacter = Cast<APstCharacter>(ActorInfo->AvatarActor))
		{
			AvatarCharacter->SetCombatMode(true);
		}

		if (UPstCharacterMovementComponent* CharacterMovementComponent = AvatarActor->FindComponentByClass<UPstCharacterMovementComponent>())
		{
			CharacterMovementComponent->AimDownSights();
		}
	}
}

void UPstGameplayAbility_AimDownSights::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor)
	{
		if (UPstCharacterMovementComponent* CharacterMovementComponent = AvatarActor->FindComponentByClass<UPstCharacterMovementComponent>())
		{
			CharacterMovementComponent->UnAimDownSights();
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UPstGameplayAbility_AimDownSights::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (APstCharacter* AvatarCharacter = Cast<APstCharacter>(ActorInfo->AvatarActor))
	{
		AvatarCharacter->SetCombatMode(false);
	}

	StopAimDownSights();
}

void UPstGameplayAbility_AimDownSights::StartAimDownSights()
{
	if (AimDownSightsLengthCurve && AimDownSightsLocationCurve)
	{
		FOnTimelineFloat AimDownSightsLengthCurveCallback;
		AimDownSightsLengthCurveCallback.BindUFunction(this, TEXT("TickAimDownSightsLength"));
		AimDownSightsTimeline.AddInterpFloat(AimDownSightsLengthCurve, AimDownSightsLengthCurveCallback);

		FOnTimelineVector AimDownSightsLocationCurveCallback;
		AimDownSightsLocationCurveCallback.BindUFunction(this, TEXT("TickAimDownSightsLocation"));
		AimDownSightsTimeline.AddInterpVector(AimDownSightsLocationCurve, AimDownSightsLocationCurveCallback);

		FOnTimelineEvent TimelineFinishedEvent;
		TimelineFinishedEvent.BindUFunction(this, TEXT("FinishedTimeline"));
		AimDownSightsTimeline.SetTimelineFinishedFunc(TimelineFinishedEvent);

		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(TimelineTickTimerHandle, FTimerDelegate::CreateLambda(
				[this]() -> void
				{
					AimDownSightsTimeline.TickTimeline(GetWorld()->GetDeltaSeconds());
				}), World->GetDeltaSeconds(), true);
		}

		AimDownSightsTimeline.Play();
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UPstGameplayAbility_AimDownSights::StopAimDownSights()
{
	if (AimDownSightsLengthCurve && AimDownSightsLocationCurve)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(TimelineTickTimerHandle, FTimerDelegate::CreateLambda(
				[this]() -> void
				{
					AimDownSightsTimeline.TickTimeline(GetWorld()->GetDeltaSeconds());
				}), World->GetDeltaSeconds(), true);
		}

		AimDownSightsTimeline.Reverse();
	}
	else
	{
		EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UPstGameplayAbility_AimDownSights::TickAimDownSightsLength(float Length)
{
	USpringArmComponent* CharacterSpringArm = GetActorInfo().AvatarActor->FindComponentByClass<USpringArmComponent>();
	if (CharacterSpringArm)
	{
		CharacterSpringArm->TargetArmLength = Length;
	}
}

void UPstGameplayAbility_AimDownSights::TickAimDownSightsLocation(FVector Location)
{
	USpringArmComponent* CharacterSpringArm = GetActorInfo().AvatarActor->FindComponentByClass<USpringArmComponent>();
	if (CharacterSpringArm)
	{
		CharacterSpringArm->SocketOffset = Location;
	}
}

void UPstGameplayAbility_AimDownSights::FinishedTimeline()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(TimelineTickTimerHandle);
	}
}

void UPstGameplayAbility_AimDownSights::HandleMovementModeChange(EMovementMode NewMovementMode)
{
	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
}
