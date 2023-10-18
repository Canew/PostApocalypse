


#include "UI/PstUserWidget_DamageDisplay.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

void UPstUserWidget_DamageDisplay::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		float DestructTime = 1.0f;
		if (FadeAnimation)
		{
			DestructTime = FadeAnimation->GetEndTime();
			PlayAnimation(FadeAnimation);
		}
		PlayerController->GetWorldTimerManager().SetTimer(DestructTimerHandle, this, &UPstUserWidget_DamageDisplay::OnFadeAnimationFinished, DestructTime, false);
	}
	else
	{
		RemoveFromParent();
	}
}

void UPstUserWidget_DamageDisplay::SetDamageAmount(float InDamageAmount)
{
	DamageAmount = InDamageAmount;

	if (DamageText)
	{
		DamageText->SetText(FText::FromString(FString::SanitizeFloat(DamageAmount, 0)));
	}
}

void UPstUserWidget_DamageDisplay::SetDamageColor(FSlateColor DamageColor)
{
	if (DamageText)
	{
		DamageText->SetColorAndOpacity(DamageColor);
	}
}

void UPstUserWidget_DamageDisplay::OnFadeAnimationFinished()
{
	RemoveFromParent();
}
