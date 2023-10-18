

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PstGameMode.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API APstGameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
