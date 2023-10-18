


#include "GameplayAbility/PstGameplayAbility_Reload.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "PstGameplayTags.h"
#include "Weapon/PstWeaponComponent.h"
#include "Weapon/PstWeapon.h"
#include "GameplayAbility/PstAttributeSet_Weapon.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstGameplayAbility_Reload)

UPstGameplayAbility_Reload::UPstGameplayAbility_Reload()
{
	
}

bool UPstGameplayAbility_Reload::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	if (!IsValid(ReloadMontage))
	{
		return false;
	}

	return true;
}

void UPstGameplayAbility_Reload::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (UAbilityTask_PlayMontageAndWait* PlayReloadMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, ReloadMontage))
	{
		PlayReloadMontageTask->OnCompleted.AddDynamic(this, &ThisClass::HandleMontageComplete);
		PlayReloadMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::HandleMontageBlendOut);
		PlayReloadMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::HandleMontageInterrupt);
		PlayReloadMontageTask->OnCancelled.AddDynamic(this, &ThisClass::HandleMontageCancel);
		PlayReloadMontageTask->ReadyForActivation();

		if (UAbilityTask_WaitGameplayEvent* WaitReloadEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, PstGameplayTags::GameplayEvent_Reload))
		{
			WaitReloadEventTask->EventReceived.AddDynamic(this, &ThisClass::HandleReloadEventReceived);
			WaitReloadEventTask->ReadyForActivation();
		}
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UPstGameplayAbility_Reload::HandleMontageComplete()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Reload::HandleMontageBlendOut()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Reload::HandleMontageInterrupt()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Reload::HandleMontageCancel()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}

void UPstGameplayAbility_Reload::HandleReloadEventReceived(FGameplayEventData Payload)
{
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
	if (IsValid(WeaponAbilitySystemComponent))
	{
		float MaxAmmoValue = WeaponAbilitySystemComponent->GetNumericAttribute(UPstAttributeSet_Weapon::GetMaxAmmoAttribute());
		WeaponAbilitySystemComponent->SetNumericAttributeBase(UPstAttributeSet_Weapon::GetAmmoAttribute(), MaxAmmoValue);
	}
}
