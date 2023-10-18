


#include "GameplayAbility/PstGameplayAbility_Crouch.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayAbility/PstGameplayAbility_Slide.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstGameplayAbility_Crouch)

bool UPstGameplayAbility_Crouch::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (ACharacter* AvatarCharacter = Cast<ACharacter>(ActorInfo->AvatarActor))
	{
		if (UCharacterMovementComponent* CharacterMovementComponent = AvatarCharacter->GetCharacterMovement())
		{
			return !CharacterMovementComponent->IsFalling();
		}
	}

	return false;
}

void UPstGameplayAbility_Crouch::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
		else
		{
			AvatarCharacter->Crouch();
			TryActivateSlideAbility();
		}

	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UPstGameplayAbility_Crouch::TryActivateSlideAbility()
{
	if (!IsValid(SlideAbilityClass))
	{
		return;
	}

	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		AbilitySystemComponent->TryActivateAbilityByClass(SlideAbilityClass);
	}
}
