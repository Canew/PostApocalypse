


#include "Weapon/PstProjectile.h"
#include "Components/SphereComponent.h"
#include "Character/PstCharacter.h"
#include "Player/PstPlayerController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "GenericTeamAgentInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstProjectile)

APstProjectile::APstProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetGenerateOverlapEvents(false);
	ProjectileCollision->SetCollisionProfileName(TEXT("Projectile"));
	SetRootComponent(ProjectileCollision);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetGenerateOverlapEvents(false);
	ProjectileMesh->SetupAttachment(ProjectileCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

void APstProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileCollision->OnComponentHit.AddDynamic(this, &ThisClass::HandleProjectileCollisionHit);
}

void APstProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void APstProjectile::HandleProjectileCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority())
	{
		return;
	}

	if (OtherActor == this || OtherActor == GetOwner() || OtherActor == GetInstigator())
	{
		return;
	}

	UAbilitySystemComponent* OwnerAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	UAbilitySystemComponent* OtherAbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);

	if (IsValid(OwnerAbilitySystemComponent))
	{
		if (IsValid(OtherAbilitySystemComponent))
		{
			for (TSubclassOf<UGameplayEffect> ImpactGameplayEffectClass : ImpactGameplayEffectClasses)
			{
				FGameplayEffectContextHandle EffectContext = OwnerAbilitySystemComponent->MakeEffectContext();
				EffectContext.AddHitResult(Hit, true);
				FGameplayEffectSpec GameplayEffectSpec(ImpactGameplayEffectClass.GetDefaultObject(), EffectContext);
				OwnerAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(GameplayEffectSpec, OtherAbilitySystemComponent, OwnerAbilitySystemComponent->ScopedPredictionKey);
			}
		}

		FGameplayCueParameters GameplayCueParameters;
		GameplayCueParameters.EffectContext = OwnerAbilitySystemComponent->MakeEffectContext();
		GameplayCueParameters.EffectCauser = this;
		GameplayCueParameters.Instigator = GetOwner();
		GameplayCueParameters.Location = Hit.ImpactPoint.IsZero() ? GetActorLocation() : Hit.ImpactPoint;
		GameplayCueParameters.Normal = GetActorRotation().Vector();
		GameplayCueParameters.PhysicalMaterial = Hit.PhysMaterial;
		OwnerAbilitySystemComponent->ExecuteGameplayCue(ImpactGameplayCueTag, GameplayCueParameters);

		if (IsValid(GetInstigator()) && IsValid(OtherAbilitySystemComponent))
		{
			if (APstPlayerController* PlayerController = GetInstigator()->GetController<APstPlayerController>())
			{
				bool bCriticalHit = false;
				if (APstCharacter* OtherCharacter = Cast<APstCharacter>(OtherActor))
				{
					bCriticalHit = OtherCharacter->IsWeakPointBody(Hit.BoneName);
				}

				PlayerController->PlayHitSound2D(bCriticalHit);
			}
		}

		ProcessOwnerClientProcedure(OtherActor);
	}

	Destroy();
}

void APstProjectile::ProcessOwnerClientProcedure_Implementation(AActor* OtherActor)
{
	if (APstCharacter* OtherCharacter = Cast<APstCharacter>(OtherActor))
	{
		OtherCharacter->FlickHitFeedbackMaterial();
	}
}
