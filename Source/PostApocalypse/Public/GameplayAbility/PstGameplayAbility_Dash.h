

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/PstGameplayAbility.h"
#include "PstGameplayAbility_Dash.generated.h"

UENUM()
enum class EPstDashDirection : uint8
{
	Forward,
	Backward,
	Left,
	Right
};

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstGameplayAbility_Dash : public UPstGameplayAbility
{
	GENERATED_BODY()

public:
	UPstGameplayAbility_Dash();

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* DashMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EPstDashDirection DashDirection;
};
