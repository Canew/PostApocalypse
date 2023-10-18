


#include "GameplayAbility/PstGameplayAbility_Dash.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstGameplayAbility_Dash)

UPstGameplayAbility_Dash::UPstGameplayAbility_Dash()
{
	
}

bool UPstGameplayAbility_Dash::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor);
	if (!IsValid(DashMontage) || !IsValid(AvatarPawn))
	{
		return false;
	}

	return true;
}

void UPstGameplayAbility_Dash::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	


	if (UAbilityTask_PlayMontageAndWait* PlaySlideMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, DashMontage))
	{
		APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor);
		FRotator ControlRotation = AvatarPawn->GetActorRotation();
		switch (DashDirection)
		{
		case EPstDashDirection::Forward:
			ControlRotation = FRotationMatrix(AvatarPawn->GetControlRotation()).GetUnitAxis(EAxis::X).Rotation();
			break;
				
		case EPstDashDirection::Backward:
			ControlRotation = (FRotationMatrix(AvatarPawn->GetControlRotation()).GetUnitAxis(EAxis::X) * -1.0f).Rotation();
			break;

		case EPstDashDirection::Left:
			ControlRotation = (FRotationMatrix(AvatarPawn->GetControlRotation()).GetUnitAxis(EAxis::Y) * -1.0f).Rotation();
			break;

		case EPstDashDirection::Right:
			ControlRotation = FRotationMatrix(AvatarPawn->GetControlRotation()).GetUnitAxis(EAxis::Y).Rotation();
			break;
		}

		ControlRotation.Roll = 0.0f;
		ControlRotation.Pitch = 0.0f;
		AvatarPawn->SetActorRotation(ControlRotation);

		PlaySlideMontageTask->OnCompleted.AddDynamic(this, &ThisClass::HandleMontageComplete);
		PlaySlideMontageTask->OnBlendOut.AddDynamic(this, &ThisClass::HandleMontageBlendOut);
		PlaySlideMontageTask->OnInterrupted.AddDynamic(this, &ThisClass::HandleMontageInterrupt);
		PlaySlideMontageTask->OnCancelled.AddDynamic(this, &ThisClass::HandleMontageCancel);
		PlaySlideMontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	}
}

void UPstGameplayAbility_Dash::HandleMontageComplete()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Dash::HandleMontageBlendOut()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Dash::HandleMontageInterrupt()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

void UPstGameplayAbility_Dash::HandleMontageCancel()
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, true);
}
