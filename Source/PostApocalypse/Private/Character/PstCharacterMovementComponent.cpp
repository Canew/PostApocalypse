


#include "Character/PstCharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstCharacterMovementComponent)

UPstCharacterMovementComponent::UPstCharacterMovementComponent()
{
	bIsSprinting = false;
	SprintSpeedMultiplier = 1.3f;
	AimDownSightsSpeedMultiplier = 0.4f;
}

float UPstCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();

	if (bIsSprinting)
	{
		MaxSpeed *= SprintSpeedMultiplier;
	}

	if (bAimDownSights)
	{
		MaxSpeed *= AimDownSightsSpeedMultiplier;
	}

	return MaxSpeed;
}

bool UPstCharacterMovementComponent::IsSprinting() const
{
	return bIsSprinting;
}

void UPstCharacterMovementComponent::Sprint()
{
	bIsSprinting = true;
}

void UPstCharacterMovementComponent::UnSprint()
{
	bIsSprinting = false;
}

float UPstCharacterMovementComponent::GetSprintSpeedMultiplier() const
{
	return SprintSpeedMultiplier;
}

bool UPstCharacterMovementComponent::IsAimDownSights()
{
	return bAimDownSights;
}

void UPstCharacterMovementComponent::AimDownSights()
{
	bAimDownSights = true;
}

void UPstCharacterMovementComponent::UnAimDownSights()
{
	bAimDownSights = false;
}
