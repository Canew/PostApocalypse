

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "PstProjectile.generated.h"

UCLASS()
class POSTAPOCALYPSE_API APstProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	APstProjectile();

	virtual void PostInitializeComponents();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE virtual class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; };

protected:
	UFUNCTION()
	virtual void HandleProjectileCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(Client, Reliable)
	virtual void ProcessOwnerClientProcedure(AActor* OtherActor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USphereComponent> ProjectileCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Gameplay Effect")
	TArray<TSubclassOf<class UGameplayEffect>> ImpactGameplayEffectClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile|Gameplay Cue")
	FGameplayTag ImpactGameplayCueTag;
};
