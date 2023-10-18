

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PstHUD.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API APstHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	virtual void InitializeDefaultWidgets();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TArray<TSubclassOf<class UUserWidget>> DefaultWidgetClasses;
};
