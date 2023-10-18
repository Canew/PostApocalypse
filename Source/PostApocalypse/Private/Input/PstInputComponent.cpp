


#include "Input/PstInputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputSubsystems.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstInputComponent)

UPstInputComponent::UPstInputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UPstInputComponent::AddInputMappings(const UPstInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to add something from your input config if required
}

void UPstInputComponent::RemoveInputMappings(const UPstInputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	check(InputConfig);
	check(InputSubsystem);

	// Here you can handle any custom logic to remove input mappings that you may have added above
}

void UPstInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}

	BindHandles.Reset();
}

