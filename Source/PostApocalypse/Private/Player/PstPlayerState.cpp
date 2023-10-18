


#include "Player/PstPlayerState.h"
#include "GameplayAbility/PstAbilitySystemComponent.h"
#include "GameplayAbility/PstAttributeSet_Character.h"
#include "GameplayAbility/PstAbilitySet.h"
#include "Player/PstPlayerController.h"
#include "Character/PstCharacterSetting.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstPlayerState)

APstPlayerState::APstPlayerState(const FObjectInitializer& ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UPstAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	CreateDefaultSubobject<UPstAttributeSet_Character>(TEXT("AttributeSet"));

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

void APstPlayerState::PreInitializeComponents()
{
	Super::InitializeComponents();
}

void APstPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);

	if (AActor* AvatarActor = GetPawn())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, AvatarActor);
	}
}

APstPlayerController* APstPlayerState::GetPstPlayerController() const
{
	return Cast<APstPlayerController>(GetOwner());
}

UAbilitySystemComponent* APstPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APstPlayerState::SetCharacterSetting(const UPstCharacterSetting* InCharacterSetting)
{
	check(InCharacterSetting);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (IsValid(CharacterSetting.Get()))
	{
		return;
	}

	CharacterSetting = InCharacterSetting;

	for (const UPstAbilitySet* AbilitySet : CharacterSetting->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}

