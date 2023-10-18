

#pragma once

#include "CoreMinimal.h"
#include "PstGameMode.h"
#include "PstGameMode_Annihilation.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API APstGameMode_Annihilation : public APstGameMode
{
	GENERATED_BODY()

public:
	APstGameMode_Annihilation();

	virtual void StartMatch() override;

protected:
	void SpawnEnemy();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pst|GameMode")
	int32 TotalEnemyCount;
	UPROPERTY(EditDefaultsOnly, Category = "Pst|GameMode")
	int32 MaxEnemyCount;
	int32 SpawnedEnemyCount;

	UPROPERTY(EditDefaultsOnly, Category = "Pst|GameMode")
	float SpawnRangeRadius;

	FTimerHandle SpawnTimerHandle;
	UPROPERTY(EditDefaultsOnly, Category = "Pst|GameMode")
	float SpawnDelay;


	UPROPERTY(EditDefaultsOnly, Category = "Pst|GameMode")
	TArray<TSubclassOf<class AActor>> EnemyClasses;
};
