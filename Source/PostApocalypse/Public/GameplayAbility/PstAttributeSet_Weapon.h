

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/PstAttributeSet.h"
#include "PstAttributeSet_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstAttributeSet_Weapon : public UPstAttributeSet
{
	GENERATED_BODY()

public:
	UPstAttributeSet_Weapon();

	ATTRIBUTE_ACCESSORS(UPstAttributeSet_Weapon, AttackPower);
	ATTRIBUTE_ACCESSORS(UPstAttributeSet_Weapon, Ammo);
	ATTRIBUTE_ACCESSORS(UPstAttributeSet_Weapon, MaxAmmo);

protected:
	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Ammo(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxAmmo(const FGameplayAttributeData& OldValue);

protected:
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackPower, Category = "Pst|Attack", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPower;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Ammo, Category = "Pst|Ammo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Ammo;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxAmmo, Category = "Pst|Ammo", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAmmo;
};
