

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "PstWeapon.generated.h"

UENUM(BlueprintType)
enum class EPstWeaponType : uint8
{
	Pistol,
	SMG,
	SG,
	AR,
	SR
};

USTRUCT(BlueprintType)
struct FPstWeaponRecoilData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float VerticalRecoilMagnitude;

	UPROPERTY(EditDefaultsOnly)
	float HorizontalRecoilMagnitude;

	UPROPERTY(EditDefaultsOnly)
	float RecoilTime;

	UPROPERTY(EditDefaultsOnly)
	float RecoilRecoveryTime;

	FTimerHandle RecoilTimerHandle;
	bool bIsRecoiling;

	FTimerHandle RecoilRecoveryTimerHandle;
	bool bIsRecoilRecovering;
};

UCLASS()
class POSTAPOCALYPSE_API APstWeapon : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	APstWeapon();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Fire();

	FORCEINLINE EPstWeaponType GetWeaponType() const { return WeaponType; }
	FTransform GetLeftHandSocketTransform() const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return WeaponAbilitySystem; }

protected:
	virtual void StartRecoil();
	virtual void TickRecoil();
	virtual void EndRecoil();

	virtual void StartRecoilRecovery();
	virtual void TickRecoilRecovery();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAbilitySystemComponent> WeaponAbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UAttributeSet> WeaponAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Type")
	EPstWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Socket")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Socket")
	FName LeftHandAttachSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Projectile")
	TSubclassOf<class APstProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Gameplay Cue")
	FGameplayTag MuzzleFlashGameplayCueTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Recoil")
	FPstWeaponRecoilData WeaponRecoilData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attribute")
	TSubclassOf<class UGameplayEffect> WeaponAttributeGameplayEffect;
};
