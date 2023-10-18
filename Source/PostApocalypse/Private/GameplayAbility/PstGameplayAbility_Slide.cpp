


#include "GameplayAbility/PstGameplayAbility_Slide.h"
#include "GameFramework/Character.h"
#include "Character/PstCharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstGameplayAbility_Slide)

UPstGameplayAbility_Slide::UPstGameplayAbility_Slide()
{
	MinSlideVelocityThreshold = 200.0f;
}

bool UPstGameplayAbility_Slide::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (!IsValid(SlideMontage))
	{
		return false;
	}

	if (ACharacter* AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		if (UCharacterMovementComponent * CharacterMovementComponent = AvatarCharacter->GetCharacterMovement())
		{
			return !CharacterMovementComponent->IsFalling() && CharacterMovementComponent->Velocity.Size() > MinSlideVelocityThreshold;
		}
	}

	return false;
}

void UPstGameplayAbility_Slide::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	float TranslationScale = 1.0f;;
	if (AActor* AvatarActor = ActorInfo->AvatarActor.Get())
	{
		if (UPstCharacterMovementComponent* CharacterMovementComponent = AvatarActor->FindComponentByClass<UPstCharacterMovementComponent>())
		{
			if (CharacterMovementComponent->IsSprinting())
			{
				TranslationScale = CharacterMovementComponent->GetSprintSpeedMultiplier();
			}
		}
	}

	if (UAbilityTask_PlayMontageAndWait* PlaySlideMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SlideMontage, 1.0f, NAME_None, true, TranslationScale))
	{
		PlaySlideMontageTask->OnCompleted.AddDynamic(this, &ThisClass::HandleMontageComplete);
		PlaySlideMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::HandleMontageBlendOut);
		PlaySlideMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::HandleMontageInterrupt);
		PlaySlideMontageTask->OnCancelled.AddDynamic(this, &ThisClass::HandleMontageCancel);
		PlaySlideMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UPstGameplayAbility_Slide::HandleMontageComplete()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Slide::HandleMontageBlendOut()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Slide::HandleMontageInterrupt()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Slide::HandleMontageCancel()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}
