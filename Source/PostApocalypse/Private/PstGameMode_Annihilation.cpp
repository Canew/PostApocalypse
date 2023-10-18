


#include "PstGameMode_Annihilation.h"
#include "EngineUtils.h"
#include "NavigationSystem.h"
#include "AIController.h"

APstGameMode_Annihilation::APstGameMode_Annihilation()
{
	TotalEnemyCount = 0;
	MaxEnemyCount = 0;
	SpawnedEnemyCount = 0;
	SpawnRangeRadius = 1000.0f;
	SpawnDelay = 1.0f;
}

void APstGameMode_Annihilation::StartMatch()
{
	Super::StartMatch();

	FTimerDelegate SpawnTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::SpawnEnemy);
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, SpawnTimerDelegate, SpawnDelay, true);
}

void APstGameMode_Annihilation::SpawnEnemy()
{
	int32 CurrentEnemyCount = 0;
	for (AAIController* AIController : TActorRange<AAIController>(GetWorld()))
	{
		APawn* AIPawn = AIController->GetPawn();
		if (IsValid(AIPawn))
		{
			CurrentEnemyCount++;
		}
	}

	if (CurrentEnemyCount < MaxEnemyCount)
	{
		int32 EnemyClassIndex = FMath::RandRange(0, EnemyClasses.Num() - 1);
		TSubclassOf<AActor> EnemyClass = EnemyClasses[EnemyClassIndex];
		if (!EnemyClass)
		{
			return;
		}

		TArray<APlayerController*> PlayerControllerArray;
		for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
		{
			PlayerControllerArray.Add(PlayerController);
		}

		int32 TargetPlayerControllerIndex = FMath::RandRange(0, PlayerControllerArray.Num() - 1);
		APlayerController* TargetPlayerController = PlayerControllerArray[TargetPlayerControllerIndex];
		if (!IsValid(TargetPlayerController))
		{
			return;
		}

		APawn* TargetPlayerPawn = TargetPlayerController->GetPawn();
		if (!IsValid(TargetPlayerPawn))
		{
			return;
		}

		UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (!IsValid(NavigationSystem))
		{
			return;
		}

		FNavLocation NavLocation;
		NavigationSystem->GetRandomReachablePointInRadius(TargetPlayerPawn->GetActorLocation(), SpawnRangeRadius, NavLocation);

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(NavLocation.Location);

		FVector SpawnDirection = TargetPlayerPawn->GetActorLocation() - NavLocation.Location;
		SpawnDirection.Normalize();
		SpawnDirection.Z = 0.0f;
		SpawnTransform.SetRotation(SpawnDirection.ToOrientationQuat());
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		if (AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnTransform, ActorSpawnParameters))
		{
			SpawnedEnemyCount++;
		}
	}
}
