


#include "Animation/PstAnimInstance.h"
#include "GameFramework/Character.h"
#include "Character/PstCharacterMovementComponent.h"
#include "Weapon/PstWeaponComponent.h"
#include "Weapon/PstWeapon.h"	

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstAnimInstance)

void UPstAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());
	if (Character.IsValid())
	{
		MovementComponent = Character->GetCharacterMovement<UPstCharacterMovementComponent>();
	}
}

void UPstAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UPstAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	FName FullBodyCurveName = TEXT("FullBody");
	bFullBody = GetCurveValue(FullBodyCurveName) > KINDA_SMALL_NUMBER;

	if (Character.IsValid() && MovementComponent.IsValid())
	{
		if (MovementComponent->bOrientRotationToMovement)
		{
			ControlRoll = 0.0f;
			ControlPitch = 0.0f;
			ControlYaw = 0.0f;
		}
		else
		{
			FRotator RotatorDeltaActorAndControl = Character->GetBaseAimRotation() - Character->GetActorRotation();
			RotatorDeltaActorAndControl.Normalize();
			ControlRoll = RotatorDeltaActorAndControl.Roll;
			ControlPitch = RotatorDeltaActorAndControl.Pitch;
			ControlYaw = RotatorDeltaActorAndControl.Yaw;
		}

		if (!RotationLastTick.IsZero())
		{
			FRotator RotationDelta = RotationLastTick - Character->GetActorRotation();
			RotationDelta.Normalize();

			RotationYawDelta = FMath::FInterpTo(RotationYawDelta, RotationDelta.Yaw / (4.0f * DeltaSeconds), DeltaSeconds, RotationYawDeltaInterpSpeed);
		}

		RotationLastTick = Character->GetActorRotation();

		Velocity = MovementComponent->Velocity;
		GroundSpeed = Velocity.Size();

		FVector CurrentAcceleration = MovementComponent->GetCurrentAcceleration();
		ShouldMove = (!CurrentAcceleration.IsNearlyZero()) && (GroundSpeed > 1.0f);

		IsAccelerating = MovementComponent->GetCurrentAcceleration().Size() > KINDA_SMALL_NUMBER;
		IsFalling = MovementComponent->IsFalling();
		IsCrouching = MovementComponent->IsCrouching();
		IsSprinting = MovementComponent->IsSprinting();
		IsAimDownSights = MovementComponent->IsAimDownSights();

		FVector ForwardVector = FVector(Character->GetActorForwardVector().X, Character->GetActorForwardVector().Y, 0.0f);
		FVector MoveVector = FVector(Character->GetVelocity().X, Character->GetVelocity().Y, 0.0f);
		ForwardVector.Normalize();
		MoveVector.Normalize();
		if (GroundSpeed > 0.0f)
		{
			MoveDirection = FMath::RadiansToDegrees(FMath::Acos(ForwardVector | MoveVector));
		}

		float DistanceToFrontBackPlane = FVector::PointPlaneDist(Character->GetActorLocation() + Character->GetVelocity(), Character->GetActorLocation(), Character->GetActorRightVector());
		if (DistanceToFrontBackPlane < 0)
		{
			MoveDirection *= -1.0f;
		}

		InputVector = MovementComponent->GetLastInputVector().GetClampedToSize(0.0f, 1.0f);
		InputForwardDelta = (Character->GetActorRotation() - InputVector.Rotation()).Yaw;

		bShouldDoFootIK = ShouldDoFootIK();
		bShouldDoHandIK = ShouldDoHandIK();
		LeftHandIKTransform = GetLeftHandIKTransform();
	}
}

bool UPstAnimInstance::ShouldDoFootIK() const
{
	return true;
}

bool UPstAnimInstance::ShouldDoHandIK() const
{
	if (UPstWeaponComponent* WeaponComponent = Character->FindComponentByClass<UPstWeaponComponent>())
	{
		if (WeaponComponent->GetActiveWeapon())
		{
			FName DisableHandIKCurveName = TEXT("DisableHandIK");
			return !(GetCurveValue(DisableHandIKCurveName) > KINDA_SMALL_NUMBER);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

FTransform UPstAnimInstance::GetLeftHandIKTransform() const
{

	if (UPstWeaponComponent* WeaponComponent = Character->FindComponentByClass<UPstWeaponComponent>())
	{	
		if (APstWeapon* Weapon = WeaponComponent->GetActiveWeapon())
		{
			if (USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh())
			{
				return Weapon->GetLeftHandSocketTransform().GetRelativeTransform(Character->GetMesh()->GetComponentToWorld());
			}
		}
	}

	return FTransform::Identity;
}
