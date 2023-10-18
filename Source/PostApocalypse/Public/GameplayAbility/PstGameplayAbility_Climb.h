

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/PstGameplayAbility.h"
#include "PstGameplayAbility_Climb.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstGameplayAbility_Climb : public UPstGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPstGameplayAbility_Climb();

	/** Returns true if this ability can be activated right now. Has no side effects */
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

protected:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled);

	void DoClimb();

	UFUNCTION()
	void HandleMontageCompleted();
	UFUNCTION()
	void HandleMontageBlendOut();
	UFUNCTION()
	void HandleMontageInterrupted();
	UFUNCTION()
	void HandleMontageCancelled();


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	class UAnimMontage* ClimbMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Climb")
	float MaxClimbHeight;
	float TraceDistance;
	float TraceHeight;

	float OriginalCapsuleHalfHeight;
};
