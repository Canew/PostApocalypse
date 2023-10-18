

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PstAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:
	virtual bool ShouldDoFootIK() const;
	virtual bool ShouldDoHandIK() const;
	virtual FTransform GetLeftHandIKTransform() const;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Reference")
	TWeakObjectPtr<class ACharacter> Character;

	UPROPERTY(BlueprintReadWrite, Category = "Reference")
	TWeakObjectPtr<class UPstCharacterMovementComponent> MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool ShouldMove;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool IsAccelerating;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool IsFalling;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool IsCrouching;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool IsSprinting;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	bool IsAimDownSights;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	FVector Velocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MoveDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	FVector InputVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float InputForwardDelta;

	/** Roll delta between control rotation and owning actor rotation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control")
	float ControlRoll;

	/** Pitch delta between control rotation and owning actor rotation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control")
	float ControlPitch;

	/** Yaw delta between control rotation and owning actor rotation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control")
	float ControlYaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control")
	float RotationYawDelta;
	float RotationYawDeltaInterpSpeed = 6.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control")
	FRotator RotationLastTick;

	/** if character should play full body animation */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	bool bFullBody;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	bool bShouldDoFootIK;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	bool bShouldDoHandIK;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	FTransform LeftHandIKTransform;
};
