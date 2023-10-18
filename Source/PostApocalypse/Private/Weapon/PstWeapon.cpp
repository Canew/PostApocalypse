


#include "Weapon/PstWeapon.h"
#include "Weapon/PstProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbility/PstAttributeSet_Weapon.h"
#include "Camera/CameraComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstWeapon)

APstWeapon::APstWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponAbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("WeaponAbilitySystem"));

	WeaponAttributeSet = CreateDefaultSubobject<UPstAttributeSet_Weapon>(TEXT("WeaponAttributeSet"));
}

void APstWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (WeaponAttributeGameplayEffect)
	{
		WeaponAbilitySystem->ApplyGameplayEffectToSelf(WeaponAttributeGameplayEffect.GetDefaultObject(), UGameplayEffect::INVALID_LEVEL, WeaponAbilitySystem->MakeEffectContext(), WeaponAbilitySystem->ScopedPredictionKey);
	}
}

void APstWeapon::BeginPlay()
{
	Super::BeginPlay();


}

void APstWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void APstWeapon::Fire()
{
	UWorld* World = GetWorld();
	APawn* PawnOwner = GetOwner<APawn>();
	if (!World || !PawnOwner)
	{
		return;
	}

	if (WeaponMesh->DoesSocketExist(MuzzleSocketName))
	{
		FTransform MuzzleSocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
		if (APstProjectile* Projectile = World->SpawnActorDeferred<APstProjectile>(ProjectileClass, MuzzleSocketTransform, this, PawnOwner, ESpawnActorCollisionHandlingMethod::AlwaysSpawn))
		{
			UProjectileMovementComponent* ProjectileMovementComponent = Projectile->GetProjectileMovement();
			check(ProjectileMovementComponent);

			if (PawnOwner->IsPlayerControlled())
			{	
				UCameraComponent* CameraComponent = PawnOwner->FindComponentByClass<UCameraComponent>();
				check(CameraComponent);

				float TraceDistance = 100000.0f;
				FVector TraceStartLocation = CameraComponent->GetComponentLocation();
				FVector TraceEndLocation = TraceStartLocation + (PawnOwner->GetControlRotation().Vector() * TraceDistance);

				FHitResult HitResult;
				FCollisionQueryParams CollisionQueryParams;
				CollisionQueryParams.AddIgnoredActor(this);
				CollisionQueryParams.AddIgnoredActor(GetOwner());
				if (World->LineTraceSingleByChannel(HitResult, TraceStartLocation, TraceEndLocation, ECC_GameTraceChannel1, CollisionQueryParams))
				{
					ProjectileMovementComponent->Velocity = HitResult.Location - MuzzleSocketTransform.GetLocation();
				}
				else
				{
					ProjectileMovementComponent->Velocity = HitResult.TraceEnd - MuzzleSocketTransform.GetLocation();
				}
			}
			else
			{
				ProjectileMovementComponent->Velocity = PawnOwner->GetControlRotation().Vector();
			}

			Projectile->FinishSpawning(MuzzleSocketTransform);

			StartRecoil();

			if (UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()))
			{
				FGameplayCueParameters GameplayCueParameters;
				GameplayCueParameters.Location = MuzzleSocketTransform.GetLocation();
				GameplayCueParameters.Normal = MuzzleSocketTransform.GetRotation().Vector();
				GameplayCueParameters.Instigator = GetOwner();
				GameplayCueParameters.EffectCauser = Projectile;
				GameplayCueParameters.EffectContext = AbilitySystemComponent->MakeEffectContext();
				AbilitySystemComponent->ExecuteGameplayCue(MuzzleFlashGameplayCueTag, GameplayCueParameters);
			}
		}
	}
}

FTransform APstWeapon::GetLeftHandSocketTransform() const
{
	if (WeaponMesh->DoesSocketExist(LeftHandAttachSocketName))
	{
		return WeaponMesh->GetSocketTransform(LeftHandAttachSocketName);
	}

	return FTransform::Identity;
}

void APstWeapon::StartRecoil()
{
	if (APawn* PawnOwner = GetOwner<APawn>())
	{
		WeaponRecoilData.bIsRecoiling = true;
			PawnOwner->GetWorldTimerManager().SetTimer(WeaponRecoilData.RecoilTimerHandle, FTimerDelegate::CreateLambda(
			[this]() -> void
			{
				WeaponRecoilData.bIsRecoiling = false;
			}), WeaponRecoilData.RecoilTime, false);

			PawnOwner->AddControllerPitchInput(-WeaponRecoilData.VerticalRecoilMagnitude);
				PawnOwner->AddControllerYawInput(FMath::RandRange(-WeaponRecoilData.HorizontalRecoilMagnitude, WeaponRecoilData.HorizontalRecoilMagnitude));

		FTimerDelegate RecoilTimerDelegate;
		RecoilTimerDelegate.BindUObject(this, &ThisClass::TickRecoil);
		PawnOwner->GetWorldTimerManager().SetTimerForNextTick(RecoilTimerDelegate);
	}
}

void APstWeapon::TickRecoil()
{
	if (APawn* PawnOwner = GetOwner<APawn>())
	{
		if (WeaponRecoilData.bIsRecoiling)
		{
			PawnOwner->AddControllerPitchInput(-WeaponRecoilData.VerticalRecoilMagnitude);
			PawnOwner->AddControllerYawInput(FMath::RandRange(-WeaponRecoilData.HorizontalRecoilMagnitude, WeaponRecoilData.HorizontalRecoilMagnitude));

			FTimerDelegate RecoilTimerDelegate;
			RecoilTimerDelegate.BindUObject(this, &ThisClass::TickRecoil);
			PawnOwner->GetWorldTimerManager().SetTimerForNextTick(RecoilTimerDelegate);
		}
		else
		{
			EndRecoil();
		}
	}
}

void APstWeapon::EndRecoil()
{
	StartRecoilRecovery();
}

void APstWeapon::StartRecoilRecovery()
{
	if (APawn* PawnOwner = GetOwner<APawn>())
	{
		WeaponRecoilData.bIsRecoilRecovering = true;
		PawnOwner->GetWorldTimerManager().SetTimer(WeaponRecoilData.RecoilRecoveryTimerHandle, FTimerDelegate::CreateLambda(
			[this]() -> void
			{
				WeaponRecoilData.bIsRecoilRecovering = false;
			}), WeaponRecoilData.RecoilRecoveryTime, false);

		PawnOwner->AddControllerPitchInput((WeaponRecoilData.VerticalRecoilMagnitude) / 2.0f);

		FTimerDelegate RecoilRecoveryTimerDelegate;
		RecoilRecoveryTimerDelegate.BindUObject(this, &ThisClass::TickRecoilRecovery);
		PawnOwner->GetWorldTimerManager().SetTimerForNextTick(RecoilRecoveryTimerDelegate);
	}
}

void APstWeapon::TickRecoilRecovery()
{
	if (APawn* PawnOwner = GetOwner<APawn>())
	{
		if (WeaponRecoilData.bIsRecoilRecovering)
		{
			PawnOwner->AddControllerPitchInput((WeaponRecoilData.VerticalRecoilMagnitude) / 2.0f);

			FTimerDelegate RecoilRecoveryTimerDelegate;
			RecoilRecoveryTimerDelegate.BindUObject(this, &ThisClass::TickRecoilRecovery);
			PawnOwner->GetWorldTimerManager().SetTimerForNextTick(RecoilRecoveryTimerDelegate);
		}
	}
}
