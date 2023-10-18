

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/PstGameplayAbility.h"
#include "PstGameplayAbility_Fire.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstGameplayAbility_Fire : public UPstGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPstGameplayAbility_Fire();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	UFUNCTION()
	void HandleMontageComplete();
	UFUNCTION()
	void HandleMontageBlendOut();
	UFUNCTION()
	void HandleMontageInterrupt();
	UFUNCTION()
	void HandleMontageCancel();

	UFUNCTION()
	void HandleLaunchEventReceive(FGameplayEventData Payload);

	void DoFire();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* FireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> WeaponCostGameplayEffectClass;
};
