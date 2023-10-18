

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PstCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UPstCharacterMovementComponent();

	virtual float GetMaxSpeed() const override;

	virtual bool IsSprinting() const;
	virtual void Sprint();
	virtual void UnSprint();
	virtual float GetSprintSpeedMultiplier() const;

	virtual bool IsAimDownSights();
	virtual void AimDownSights();
	virtual void UnAimDownSights();

protected:
	bool bIsSprinting;
	float SprintSpeedMultiplier;

	bool bAimDownSights;
	float AimDownSightsSpeedMultiplier;
};
