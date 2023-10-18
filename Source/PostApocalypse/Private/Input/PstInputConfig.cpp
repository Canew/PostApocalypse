


#include "Input/PstInputConfig.h"
#include "InputMappingContext.h"
#include "InputAction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstInputConfig)

UPstInputConfig::UPstInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UPstInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FPstInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}

const UInputAction* UPstInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FPstInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}
