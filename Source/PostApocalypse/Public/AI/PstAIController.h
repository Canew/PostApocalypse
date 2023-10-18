

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "PstAIController.generated.h"

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API APstAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	APstAIController(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;

	void RunAI();
	void RestartAI();
	void StopAI();

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	virtual void InitializeHomeLocation();
	virtual void TargetClosestHostileActor();
	virtual void ExpireTarget();

protected:
	UPROPERTY(EditDefaultsOnly)
	float ControlRotationInterpSpeed;

	UPROPERTY()
	class UAISenseConfig_Sight* SightSenseConfig;
	UPROPERTY()
	class UAISenseConfig_Damage* DamageSenseConfig;
	UPROPERTY()
	class UAISenseConfig_Hearing* HearingSenseConfig;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBlackboardData* BlackboardData;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	FTimerHandle TargetExpirationTimerHandle;
	FName HomeLocationKeyName;
	FName TargetActorKeyName;
};
