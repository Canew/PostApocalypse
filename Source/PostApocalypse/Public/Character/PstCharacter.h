

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "PstCharacter.generated.h"

UCLASS()
class POSTAPOCALYPSE_API APstCharacter : public ACharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APstCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnRep_PlayerState() override;

public:
	virtual void PossessedBy(AController* NewController) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	template <typename T>
	T* GetAbilitySystemComponent() const
	{
		return Cast<T>(GetAbilitySystemComponent());
	}

protected:
	void MoveInput(const FInputActionValue& InputActionValue);
	void LookInput(const FInputActionValue& InputActionValue);

	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);

	void HandleHealthChanged(const FOnAttributeChangeData& Data);
	void BeginDeath();
	void FinishDeath();

	FTimerHandle DissolveTimerHandle;

protected:
	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UPstWeaponComponent> WeaponComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = "Perception")
	class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSource;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TArray<TObjectPtr<class UInputMappingContext>> InputMappings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting")
	TObjectPtr<class UPstCharacterSetting> CharacterSetting;

public:
	virtual bool IsCombatMode() const;
	virtual void SetCombatMode(bool NewCombatMode, bool bAutoDisable = false);

protected:
	bool bIsCombatMode;
	FTimerHandle ClearCombatModeTimerHandle;
	float ClearCombatModeTime;

public:
	virtual void FlickHitFeedbackMaterial();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Material")
	TObjectPtr<UMaterialInterface> HitFeedbackMaterial;

	FTimerHandle FlickHitFeedbackMaterialTimerHandle;
	float FlickHitFeedbackMaterialTime = 0.02f;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Pst|Character")
	float GetDamageMultiplierByName(const FName BoneName) const;

	UFUNCTION(BlueprintNativeEvent, Category = "Pst|Character")
	bool IsWeakPointBody(const FName BoneName) const;

public:
	void SetDissolveOpacity(float NewDissolveOpacity);

protected:
	UFUNCTION()
	void OnRep_DissolveOpacity();

	UPROPERTY(ReplicatedUsing = OnRep_DissolveOpacity)
	float DissolveOpacity = 1.0f;
};
