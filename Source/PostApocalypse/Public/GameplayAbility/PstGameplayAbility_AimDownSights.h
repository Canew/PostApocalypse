

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/PstGameplayAbility.h"
#include "Components/TimeLineComponent.h"
#include "PstGameplayAbility_AimDownSights.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstGameplayAbility_AimDownSights : public UPstGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void StartAimDownSights();
	virtual void StopAimDownSights();

	UFUNCTION()
	virtual void TickAimDownSightsLength(float Length);
	UFUNCTION()
	virtual void TickAimDownSightsLocation(FVector Location);
	UFUNCTION()
	virtual void FinishedTimeline();

	UFUNCTION()
	void HandleMovementModeChange(EMovementMode NewMovementMode);

protected:
	FTimeline AimDownSightsTimeline;
	FTimerHandle TimelineTickTimerHandle;

	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat* AimDownSightsLengthCurve;

	UPROPERTY(EditDefaultsOnly)
	class UCurveVector* AimDownSightsLocationCurve;


};
