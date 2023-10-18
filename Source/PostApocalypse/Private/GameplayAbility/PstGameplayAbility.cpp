


#include "GameplayAbility/PstGameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstGameplayAbility)

UPstGameplayAbility::UPstGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UPstGameplayAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	UAbilitySystemComponent* AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
	if (!AbilitySystemComponent)
	{
		return false;
	}

	bool bBlocked = false;
	switch (ActivationGroup)
	{
	case EPstAbilityActivationGroup::Independent:
		break;

	case EPstAbilityActivationGroup::Exclusive_Replaceable:
	case EPstAbilityActivationGroup::Exclusive_Blocking:
	{
		TArray<FGameplayAbilitySpec> ActivatableAbilitySpecs = AbilitySystemComponent->GetActivatableAbilities();
		for (FGameplayAbilitySpec& ActivatableAbilitySpec : ActivatableAbilitySpecs)
		{
			if (ActivatableAbilitySpec.IsActive())
			{
				if (UPstGameplayAbility* PstGameplayAbility = Cast<UPstGameplayAbility>(ActivatableAbilitySpec.Ability))
				{
					if (PstGameplayAbility->GetActivationGroup() == EPstAbilityActivationGroup::Exclusive_Blocking)
					{
						bBlocked = true;
						break;
					}
				}
			}
		}
		break;
	}

	default:
		return false;
	}

	if (bBlocked)
	{
		return false;
	}

	return true;
}

void UPstGameplayAbility::PreActivate(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, FOnGameplayAbilityEnded::FDelegate* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	Super::PreActivate(Handle, ActorInfo, ActivationInfo, OnGameplayAbilityEndedDelegate, TriggerEventData);

	CurrentSpecHandle = Handle;
	CurrentActorInfo = ActorInfo;
	CurrentActivationInfo = ActivationInfo;
}

void UPstGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWAbilitySystemComponentancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWAbilitySystemComponentancelled);
}

int32 UPstGameplayAbility::GetPredictionKey() const
{
	if (UAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo())
	{
		return AbilitySystemComponent->ScopedPredictionKey.Current;
	}

	return 0;
}
