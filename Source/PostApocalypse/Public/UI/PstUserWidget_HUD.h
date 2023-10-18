

#pragma once

#include "CoreMinimal.h"
#include "UI/PstUserWidget.h"
#include "GameplayEffectTypes.h"
#include "PstUserWidget_HUD.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstUserWidget_HUD : public UPstUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnAmmoChanged(const FOnAttributeChangeData& Data);

protected:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class UTextBlock> HealthText;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<class UTextBlock> AmmoText;
};
