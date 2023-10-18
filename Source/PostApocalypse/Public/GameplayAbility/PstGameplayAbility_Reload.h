

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/PstGameplayAbility.h"
#include "PstGameplayAbility_Reload.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstGameplayAbility_Reload : public UPstGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPstGameplayAbility_Reload();

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

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
	void HandleReloadEventReceived(FGameplayEventData Payload);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* ReloadMontage;
};
