


#include "Weapon/PstWeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/PstWeapon.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstWeaponComponent)

UPstWeaponComponent::UPstWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ActiveWeaponIndex = 1;
}


void UPstWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializeDefaultWeapon();
}


// Called every frame
void UPstWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

APstWeapon* UPstWeaponComponent::GetActiveWeapon() const
{
	if (ActiveWeaponIndex == 1)
	{
		return FirstWeapon;
	}
	else if (ActiveWeaponIndex == 2)
	{
		return SecondWeapon;
	}
	else
	{
		return nullptr;
	}
}

void UPstWeaponComponent::InitializeDefaultWeapon()
{
	ACharacter* OwnerCharacter = GetOwner<ACharacter>();
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	UWorld* World = OwnerCharacter->GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	if (OwnerCharacter->GetMesh()->DoesSocketExist(FirstDefaultWeaponData.WeaponAttachSocket))
	{
		FTransform SpawnTransform = OwnerCharacter->GetMesh()->GetSocketTransform(FirstDefaultWeaponData.WeaponAttachSocket);
		FActorSpawnParameters ActorSpawnParameters;
		ActorSpawnParameters.bNoFail = true;
		ActorSpawnParameters.Owner = OwnerCharacter;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (APstWeapon* SpawnedWeapon = World->SpawnActor<APstWeapon>(FirstDefaultWeaponData.WeaponClass, SpawnTransform, ActorSpawnParameters))
		{
			SpawnedWeapon->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FirstDefaultWeaponData.WeaponAttachSocket);
			FirstWeapon = SpawnedWeapon;
		}
	}
}
