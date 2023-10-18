


#include "GameplayAbility/PstGameplayAbility_Jump.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstGameplayAbility_Jump)

bool UPstGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

void UPstGameplayAbility_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (ACharacter* AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		if (AvatarCharacter->bIsCrouched)
		{
			AvatarCharacter->UnCrouch();
		}

		FTimerDelegate JumpTimerDelegate;
		JumpTimerDelegate.BindUObject(this, &ThisClass::DoJump, AvatarCharacter);
		AvatarCharacter->GetWorldTimerManager().SetTimerForNextTick(JumpTimerDelegate);
	}
}

void UPstGameplayAbility_Jump::DoJump(ACharacter* Character)
{
	Character->Jump();

	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

