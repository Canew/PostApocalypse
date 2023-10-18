

#pragma once

#include "CoreMinimal.h"
#include "UI/PstUserWidget.h"
#include "PstUserWidget_DamageDisplay.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstUserWidget_DamageDisplay : public UPstUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	virtual void SetDamageAmount(float InDamageAmount);
	virtual void SetDamageColor(FSlateColor DamageColor);

protected:
	virtual void OnFadeAnimationFinished();

protected:
	float DamageAmount;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class UTextBlock> DamageText;

	UPROPERTY(Transient, Meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> FadeAnimation;

	FTimerHandle DestructTimerHandle;
};
