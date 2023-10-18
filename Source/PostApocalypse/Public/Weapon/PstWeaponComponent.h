

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PstWeaponComponent.generated.h"

USTRUCT(BlueprintType)
struct FPstDefaultWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName WeaponAttachSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class APstWeapon> WeaponClass;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POSTAPOCALYPSE_API UPstWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPstWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintPure)
	virtual APstWeapon* GetActiveWeapon() const;

protected:
	virtual void InitializeDefaultWeapon();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class APstWeapon> FirstWeapon;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<class APstWeapon> SecondWeapon;

	UPROPERTY(meta = (ClampMin = "1", ClampMax = "2"))
	int32 ActiveWeaponIndex;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FPstDefaultWeaponData FirstDefaultWeaponData;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	FPstDefaultWeaponData SecondDefaultWeaponData;
};
