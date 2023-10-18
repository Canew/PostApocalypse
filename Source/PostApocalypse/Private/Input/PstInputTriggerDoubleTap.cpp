


#include "Input/PstInputTriggerDoubleTap.h"
#include "EnhancedPlayerInput.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstInputTriggerDoubleTap)

ETriggerState UPstInputTriggerDoubleTap::UpdateState_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	if (IsActuated(ModifiedValue) && !IsActuated(LastValue))
	{
		const float CurrentTime = PlayerInput->GetOuterAPlayerController()->GetWorld()->GetRealTimeSeconds();
		if (CurrentTime - LastTappedTime < Delay)
		{
			LastTappedTime = 0;
			return ETriggerState::Triggered;
		}
		LastTappedTime = CurrentTime;
	}
	return ETriggerState::None;
}
