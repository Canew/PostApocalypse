

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbility/PstGameplayAbility.h"
#include "PstGameplayAbility_Jump.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstGameplayAbility_Jump : public UPstGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData);

	virtual void DoJump(ACharacter* Character);
};
