

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "PstPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API APstPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APstPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	UFUNCTION(BlueprintCallable, Category = "Pst|PlayerState")
	class APstPlayerController* GetPstPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Pst|PlayerState")
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	template <typename T>
	T* GetAbilitySystemComponent() const
	{
		return Cast<T>(GetAbilitySystemComponent());
	}

	template <class T>
	const T* GetCharacterSetting() const { return Cast<T>(CharacterSetting); }

	void SetCharacterSetting(const class UPstCharacterSetting* InCharacterSetting);

protected:
	UPROPERTY()
	TObjectPtr<const class UPstCharacterSetting> CharacterSetting;

private:
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "PlayerState")
	TObjectPtr<class UPstAbilitySystemComponent> AbilitySystemComponent;
};
