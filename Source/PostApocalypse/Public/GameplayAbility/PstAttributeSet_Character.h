

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/PstAttributeSet.h"
#include "PstAttributeSet_Character.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstAttributeSet_Character : public UPstAttributeSet
{
	GENERATED_BODY()
	
public:
	UPstAttributeSet_Character();

	ATTRIBUTE_ACCESSORS(UPstAttributeSet_Character, Health);
	ATTRIBUTE_ACCESSORS(UPstAttributeSet_Character, MaxHealth);
	ATTRIBUTE_ACCESSORS(UPstAttributeSet_Character, Energy);
	ATTRIBUTE_ACCESSORS(UPstAttributeSet_Character, MaxEnergy);
	ATTRIBUTE_ACCESSORS(UPstAttributeSet_Character, Armor);

	ATTRIBUTE_ACCESSORS(UPstAttributeSet_Character, Healing);
	ATTRIBUTE_ACCESSORS(UPstAttributeSet_Character, Damage);

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Energy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxEnergy(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue);

	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;

protected:

	// The current health attribute. The health will be capped by the max health attribute. Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Pst|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute. Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Pst|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// The current Energy attribute. The Energy will be capped by the max Energy attribute. Energy is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Energy, Category = "Pst|Energy", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Energy;

	// The current max Energy attribute. Max Energy is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxEnergy, Category = "Pst|Energy", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxEnergy;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Pst|Defense", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Armor;

protected:
	//	Meta Attribute (please keep attributes that aren't 'stateful' below 

	// Incoming damage. This is mapped directly to -Health
	UPROPERTY(BlueprintReadOnly, Category = "Pst|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	// Incoming healing. This is mapped directly to +Health
	UPROPERTY(BlueprintReadOnly, Category = "Pst|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Healing;
};
