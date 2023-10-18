


#include "GameplayAbility/PstGameplayAbility_Fire.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "PstGameplayTags.h"
#include "Character/PstCharacter.h"
#include "GameplayAbility/PstAttributeSet_Weapon.h"
#include "Weapon/PstWeaponComponent.h"
#include "Weapon/PstWeapon.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstGameplayAbility_Fire)

UPstGameplayAbility_Fire::UPstGameplayAbility_Fire()
{

}

bool UPstGameplayAbility_Fire::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
}

bool UPstGameplayAbility_Fire::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	AActor* AvatarActor = GetCurrentActorInfo()->AvatarActor.Get();
	if (!IsValid(AvatarActor))
	{
		return false;
	}

	UPstWeaponComponent* WeaponComponent = AvatarActor->FindComponentByClass<UPstWeaponComponent>();
	if (!IsValid(WeaponComponent))
	{
		return false;
	}

	APstWeapon* ActiveWeapon = WeaponComponent->GetActiveWeapon();
	if (!IsValid(ActiveWeapon))
	{
		return false;
	}

	UAbilitySystemComponent* WeaponAbilitySystemComponent = ActiveWeapon->GetAbilitySystemComponent();
	if (!IsValid(WeaponAbilitySystemComponent))
	{
		return false;
	}

	if (WeaponCostGameplayEffectClass)
	{
		if (UGameplayEffect* CostGameplayEffect = WeaponCostGameplayEffectClass.GetDefaultObject())
		{
			 FGameplayEffectSpec CostGameplayEffectSpec(CostGameplayEffect, WeaponAbilitySystemComponent->MakeEffectContext());
			for (const FGameplayModifierInfo& CostGameplayEffectModifier : CostGameplayEffect->Modifiers)
			{
				float CostModifierMagnitude = 0.0f;
				if (!CostGameplayEffectModifier.ModifierMagnitude.AttemptCalculateMagnitude(CostGameplayEffectSpec, CostModifierMagnitude))
				{
					return false;
				}

				CostModifierMagnitude = FMath::Abs(CostModifierMagnitude);

				bool bFoundAttributeValue;
				float WeaponGameplayAttributeValue = WeaponAbilitySystemComponent->GetGameplayAttributeValue(CostGameplayEffectModifier.Attribute, bFoundAttributeValue);
				if (!bFoundAttributeValue || WeaponGameplayAttributeValue < CostModifierMagnitude)
				{
					return false;
				}
			}

			return true;
		}
	}
	else
	{
		return true;
	}

	return false;
}

void UPstGameplayAbility_Fire::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	AActor* AvatarActor = GetCurrentActorInfo()->AvatarActor.Get();
	if (!IsValid(AvatarActor))
	{
		return;
	}

	UPstWeaponComponent* WeaponComponent = AvatarActor->FindComponentByClass<UPstWeaponComponent>();
	if (!IsValid(WeaponComponent))
	{
		return;
	}

	APstWeapon* ActiveWeapon = WeaponComponent->GetActiveWeapon();
	if (!IsValid(ActiveWeapon))
	{
		return;
	}

	UAbilitySystemComponent* WeaponAbilitySystemComponent = ActiveWeapon->GetAbilitySystemComponent();
	if (!IsValid(WeaponAbilitySystemComponent))
	{
		return;
	}

	if (WeaponCostGameplayEffectClass)
	{
		if (UGameplayEffect* CostGameplayEffect = WeaponCostGameplayEffectClass.GetDefaultObject())
		{
			FGameplayEffectSpec CostGameplayEffectSpec(CostGameplayEffect, WeaponAbilitySystemComponent->MakeEffectContext());
			WeaponAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(CostGameplayEffectSpec, WeaponAbilitySystemComponent->ScopedPredictionKey);
		}
	}
}

void UPstGameplayAbility_Fire::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (APstCharacter* AvatarCharacter = Cast<APstCharacter>(ActorInfo->AvatarActor))
	{
		AvatarCharacter->SetCombatMode(true, true);
	}
	
	if (FireMontage)
	{
		if (UAbilityTask_PlayMontageAndWait* PlayFireMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, FireMontage))
		{
			PlayFireMontageTask->OnCompleted.AddDynamic(this, &ThisClass::HandleMontageComplete);
			PlayFireMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::HandleMontageBlendOut);
			PlayFireMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::HandleMontageInterrupt);
			PlayFireMontageTask->OnCancelled.AddDynamic(this, &ThisClass::HandleMontageCancel);
			PlayFireMontageTask->ReadyForActivation();

			if (UAbilityTask_WaitGameplayEvent* WaitLaunchGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PstGameplayTags::GameplayEvent_Launch))
			{
				WaitLaunchGameplayEvent->EventReceived.AddDynamic(this, &ThisClass::HandleLaunchEventReceive);
				WaitLaunchGameplayEvent->ReadyForActivation();
			}
		}
		else
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		}
	}
	else
	{
		DoFire();

		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UPstGameplayAbility_Fire::HandleMontageComplete()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Fire::HandleMontageBlendOut()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Fire::HandleMontageInterrupt()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Fire::HandleMontageCancel()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

void UPstGameplayAbility_Fire::HandleLaunchEventReceive(FGameplayEventData Payload)
{
	DoFire();
}

void UPstGameplayAbility_Fire::DoFire()
{
	if (AActor* AvatarActor = GetCurrentActorInfo()->AvatarActor.Get())
	{
		if (UPstWeaponComponent* WeaponComponent = AvatarActor->FindComponentByClass<UPstWeaponComponent>())
		{
			if (APstWeapon* ActiveWeapon = WeaponComponent->GetActiveWeapon())
			{
				ActiveWeapon->Fire();
			}
		}
	}
}
