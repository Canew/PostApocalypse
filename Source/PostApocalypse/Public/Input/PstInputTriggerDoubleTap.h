

#pragma once

#include "CoreMinimal.h"
#include "InputTriggers.h"
#include "PstInputTriggerDoubleTap.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, MinimalAPI, meta = (DisplayName = "Double Tap", NotInputConfigurable = "true"))
class UPstInputTriggerDoubleTap : public UInputTrigger
{
	GENERATED_BODY()
	
protected:
    virtual ETriggerState UpdateState_Implementation(const class UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime) override;
public:

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Trigger Settings", meta = (DisplayThumbnail = "false"))
    float Delay = .5f;

private:
    float LastTappedTime = 0.f;
};
