

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PstCharacterSetting.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstCharacterSetting : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPstCharacterSetting(const FObjectInitializer& ObjectInitializer);

public:

	// Class to instantiate for this pawn.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pawn")
	TSubclassOf<APawn> CharacterClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TObjectPtr<class UPstAbilitySet>> AbilitySets;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UPstInputConfig> InputConfig;
};
