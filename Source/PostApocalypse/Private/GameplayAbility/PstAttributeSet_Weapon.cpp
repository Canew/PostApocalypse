


#include "GameplayAbility/PstAttributeSet_Weapon.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Character/PstCharacter.h"
#include "Player/PstPlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstAttributeSet_Weapon)

UPstAttributeSet_Weapon::UPstAttributeSet_Weapon()
	: AttackPower(0.0f)
	, Ammo(0.0f)
	, MaxAmmo(0.0f)
{

}

void UPstAttributeSet_Weapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPstAttributeSet_Weapon, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPstAttributeSet_Weapon, Ammo, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPstAttributeSet_Weapon, MaxAmmo, COND_None, REPNOTIFY_Always);
}

void UPstAttributeSet_Weapon::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPstAttributeSet_Weapon, AttackPower, OldValue);
}

void UPstAttributeSet_Weapon::OnRep_Ammo(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPstAttributeSet_Weapon, Ammo, OldValue);
}

void UPstAttributeSet_Weapon::OnRep_MaxAmmo(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPstAttributeSet_Weapon, MaxAmmo, OldValue);
}

bool UPstAttributeSet_Weapon::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UPstAttributeSet_Weapon::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UPstAttributeSet_Weapon::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UPstAttributeSet_Weapon::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UPstAttributeSet_Weapon::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UPstAttributeSet_Weapon::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}
