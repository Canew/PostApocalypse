

#pragma once

#include "CoreMinimal.h"
#include "UI/PstUserWidget.h"
#include "PstUserWidget_Crosshair.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstUserWidget_Crosshair : public UPstUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> CenterImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> TopImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> BottomImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> LeftImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> RightImage;
};
