


#include "UI/PstHUD.h"
#include "Blueprint/UserWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstHUD)

void APstHUD::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle InitializeTimerHandle;
	FTimerDelegate InitializeTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::InitializeDefaultWidgets);
	GetWorldTimerManager().SetTimer(InitializeTimerHandle, InitializeTimerDelegate, 1.0f, false);
}

void APstHUD::InitializeDefaultWidgets()
{
	for (TSubclassOf<UUserWidget> DefaultWidgetClass : DefaultWidgetClasses)
	{
		if (UUserWidget* DefaultWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), DefaultWidgetClass))
		{
			DefaultWidget->AddToViewport();
		}
	}
}
