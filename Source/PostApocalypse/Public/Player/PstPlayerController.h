

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "PstPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API APstPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APstPlayerController();

	//~APlayerController interface
	virtual void PostInitializeComponents() override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	//~End of APlayerController interface

	UFUNCTION(BlueprintCallable, Category = "Pst|Ability")
	class UPstAbilitySystemComponent* GetPstAbilitySystemComponent() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<TObjectPtr<class UInputMappingContext>> InputMappings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<TObjectPtr<class UPstInputConfig>> InputConfigs;

#pragma region GenericTeamAgentInterface
public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	UPROPERTY()
	FGenericTeamId TeamId;

#pragma endregion

public:
	UFUNCTION(Client, Reliable)
	virtual void PlayHitSound2D(bool bCriticalHit);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> NormalHitSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> CriticalHitSound;

public:
	UFUNCTION(Client, Reliable)
	void ShowDamageDisplayWidget(AActor* TargetActor, float DamageAmount, FSlateColor DamageColor, FVector TargetLocation = FVector::ZeroVector);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Widget|Class")
	TSubclassOf<class UUserWidget> DamageDisplayWidgetClass;
};
