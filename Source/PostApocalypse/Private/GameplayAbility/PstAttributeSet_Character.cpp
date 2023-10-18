


#include "GameplayAbility/PstAttributeSet_Character.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "Character/PstCharacter.h"
#include "Player/PstPlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstAttributeSet_Character)

UPstAttributeSet_Character::UPstAttributeSet_Character()
	: Health(1.0f)
	, MaxHealth(1.0f)
	, Energy(0.0f)
	, MaxEnergy(0.0f)
	, Armor(0.0f)
{

}

void UPstAttributeSet_Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UPstAttributeSet_Character, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPstAttributeSet_Character, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPstAttributeSet_Character, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPstAttributeSet_Character, MaxEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UPstAttributeSet_Character, Armor, COND_None, REPNOTIFY_Always);
}

void UPstAttributeSet_Character::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPstAttributeSet_Character, Health, OldValue);
}

void UPstAttributeSet_Character::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPstAttributeSet_Character, MaxHealth, OldValue);
}

void UPstAttributeSet_Character::OnRep_Energy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPstAttributeSet_Character, Energy, OldValue);
}

void UPstAttributeSet_Character::OnRep_MaxEnergy(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPstAttributeSet_Character, MaxEnergy, OldValue);
}

void UPstAttributeSet_Character::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPstAttributeSet_Character, Armor, OldValue);
}

bool UPstAttributeSet_Character::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	return Super::PreGameplayEffectExecute(Data);
}

void UPstAttributeSet_Character::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;
	float MinimumEnergy = 0.0f;
	float MinimumAttackDamage = 0.0f;
	float MinimumAttackSpeed = 0.0f;

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
		const FHitResult* ContextHitResult = Context.GetHitResult();

		FName HitBoneName = ContextHitResult ? ContextHitResult->BoneName : NAME_None;
		bool bCriticalHit = false;
		float DamageMultiplier = 1.0f;
		if (APstCharacter* TargetCharacter = Cast<APstCharacter>(Data.Target.AbilityActorInfo->AvatarActor))
		{
			bCriticalHit = TargetCharacter->IsWeakPointBody(HitBoneName);
			DamageMultiplier = TargetCharacter->GetDamageMultiplierByName(HitBoneName);
		}

		// Convert into -Health and then clamp
		float DamageDone = (GetDamage() * DamageMultiplier) - GetArmor();
		SetHealth(FMath::Clamp(GetHealth() - DamageDone, MinimumHealth, GetMaxHealth()));

		// Show damage indicator to source player
		TWeakObjectPtr<APstPlayerController> SourcePlayerController = Cast<APstPlayerController>(Context.GetOriginalInstigatorAbilitySystemComponent()->AbilityActorInfo->PlayerController);
		if (IsValid(SourcePlayerController.Get()))
		{
			FColor DamageDisplayColor = bCriticalHit ? FColor::Yellow : FColor::White;
			FVector TargetLocation = ContextHitResult ? ContextHitResult->Location : FVector::ZeroVector;
			SourcePlayerController->ShowDamageDisplayWidget(Data.Target.AbilityActorInfo->AvatarActor.Get(), DamageDone, DamageDisplayColor, TargetLocation);
		}

		SetDamage(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		// Convert into +Health and then clamo
		SetHealth(FMath::Clamp(GetHealth() + GetHealing(), MinimumHealth, GetMaxHealth()));
		SetHealing(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp and fall into out of health handling below
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetEnergyAttribute())
	{
		// Clamp and fall into out of mana handling below
		SetEnergy(FMath::Clamp(GetEnergy(), MinimumEnergy, GetMaxEnergy()));
	}
}

void UPstAttributeSet_Character::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UPstAttributeSet_Character::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UPstAttributeSet_Character::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		// Make sure current health is not greater than the new max health.
		if (GetHealth() > NewValue)
		{
			UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
			check(ASC);

			ASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
	else if (Attribute == GetMaxEnergyAttribute())
	{
		// Make sure current health is not greater than the new max health.
		if (GetEnergy() > NewValue)
		{
			UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
			check(ASC);

			ASC->ApplyModToAttribute(GetEnergyAttribute(), EGameplayModOp::Override, NewValue);
		}
	}
}

void UPstAttributeSet_Character::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// Do not allow health to go negative or above max health.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetEnergyAttribute())
	{
		// Do not allow mana to go negative or above max mana.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxEnergy());
	}
	else if (Attribute == GetMaxEnergyAttribute())
	{
		// Do not allow max mana to drop below 0.
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}
