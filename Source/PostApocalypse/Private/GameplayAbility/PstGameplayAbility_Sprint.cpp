


#include "GameplayAbility/PstGameplayAbility_Sprint.h"
#include "Character/PstCharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstGameplayAbility_Sprint)

bool UPstGameplayAbility_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags))
	{
		return false;
	}

	return true;
}

void UPstGameplayAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor)
	{
		if (UPstCharacterMovementComponent* CharacterMovementComponent = AvatarActor->FindComponentByClass<UPstCharacterMovementComponent>())
		{
			CharacterMovementComponent->Sprint();
		}
	}
}

void UPstGameplayAbility_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor)
	{
		if (UPstCharacterMovementComponent* CharacterMovementComponent = AvatarActor->FindComponentByClass<UPstCharacterMovementComponent>())
		{
			CharacterMovementComponent->UnSprint();
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UPstGameplayAbility_Sprint::HandleMovementModeChange(EMovementMode NewMovementMode)
{
	EndAbility(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, true, false);
}
