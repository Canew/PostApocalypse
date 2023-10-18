


#include "UI/PstUserWidget_HUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Character/PstCharacter.h"
#include "GameplayAbility/PstAbilitySystemComponent.h"
#include "GameplayAbility/PstAttributeSet_Character.h"
#include "GameplayAbility/PstAttributeSet_Weapon.h"
#include "Weapon/PstWeaponComponent.h"
#include "Weapon/PstWeapon.h"

void UPstUserWidget_HUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (APstCharacter* OwningCharacter = GetOwningPlayerPawn<APstCharacter>())
	{
		UAbilitySystemComponent* CharacterAbilitySystemComponent = OwningCharacter->GetAbilitySystemComponent();
		if (CharacterAbilitySystemComponent)
		{
			const UPstAttributeSet_Character* CharacterAttributeSet = Cast<UPstAttributeSet_Character>(CharacterAbilitySystemComponent->GetAttributeSet(UPstAttributeSet_Character::StaticClass()));
			if (CharacterAttributeSet)
			{
				float CurrentHealth = CharacterAttributeSet->GetHealth();
				float MaxHealth = CharacterAttributeSet->GetMaxHealth();
				HealthBar->SetPercent(CurrentHealth / MaxHealth);

				FString HealthString = FString::SanitizeFloat(CurrentHealth, 0) + TEXT("  /  ") + FString::SanitizeFloat(MaxHealth, 0);
				HealthText->SetText(FText::FromString(HealthString));

				CharacterAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CharacterAttributeSet->GetHealthAttribute()).AddUObject(this, &UPstUserWidget_HUD::OnHealthChanged);
			}
		}

		if (UPstWeaponComponent* WeaponComponent = OwningCharacter->FindComponentByClass<UPstWeaponComponent>())
		{
			if (APstWeapon* ActiveWeapon = WeaponComponent->GetActiveWeapon())
			{
				UAbilitySystemComponent* WeaponAbilitySystemComponent = ActiveWeapon->GetAbilitySystemComponent();
				const UPstAttributeSet_Weapon* WeaponAttributeSet = Cast<UPstAttributeSet_Weapon>(WeaponAbilitySystemComponent->GetAttributeSet(UPstAttributeSet_Weapon::StaticClass()));
				if (WeaponAttributeSet)
				{
					float CurrentAmmo = WeaponAttributeSet->GetAmmo();
					float MaxAmmo = WeaponAttributeSet->GetMaxAmmo();
					FString AmmoString = FString::SanitizeFloat(CurrentAmmo, 0) + TEXT(" / ") + FString::SanitizeFloat(MaxAmmo, 0);
					AmmoText->SetText(FText::FromString(AmmoString));

					WeaponAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(WeaponAttributeSet->GetAmmoAttribute()).AddUObject(this, &UPstUserWidget_HUD::OnAmmoChanged);
				}
			}
		}

	}
}

void UPstUserWidget_HUD::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (APstCharacter* OwningCharacter = GetOwningPlayerPawn<APstCharacter>())
	{
		UAbilitySystemComponent* CharacterAbilitySystemComponent = OwningCharacter->GetAbilitySystemComponent();
		if (CharacterAbilitySystemComponent)
		{
			const UPstAttributeSet_Character* CharacterAttributeSet = Cast<UPstAttributeSet_Character>(CharacterAbilitySystemComponent->GetAttributeSet(UPstAttributeSet_Character::StaticClass()));
			if (CharacterAttributeSet)
			{
				float CurrentHealth = Data.NewValue;
				float MaxHealth = CharacterAttributeSet->GetMaxHealth();
				HealthBar->SetPercent(CurrentHealth / MaxHealth);

				FString HealthString = FString::SanitizeFloat(CurrentHealth, 0) + TEXT("  /  ") + FString::SanitizeFloat(MaxHealth, 0);
				HealthText->SetText(FText::FromString(HealthString));
			}
		}
	}
}

void UPstUserWidget_HUD::OnAmmoChanged(const FOnAttributeChangeData& Data)
{
	if (APstCharacter* OwningCharacter = GetOwningPlayerPawn<APstCharacter>())
	{
		if (UPstWeaponComponent* WeaponComponent = OwningCharacter->FindComponentByClass<UPstWeaponComponent>())
		{
			if (APstWeapon* ActiveWeapon = WeaponComponent->GetActiveWeapon())
			{
				UAbilitySystemComponent* WeaponAbilitySystemComponent = ActiveWeapon->GetAbilitySystemComponent();
				const UPstAttributeSet_Weapon* WeaponAttributeSet = Cast<UPstAttributeSet_Weapon>(WeaponAbilitySystemComponent->GetAttributeSet(UPstAttributeSet_Weapon::StaticClass()));
				if (WeaponAttributeSet)
				{
					float CurrentAmmo = Data.NewValue;
					float MaxAmmo = WeaponAttributeSet->GetMaxAmmo();
					FString AmmoString = FString::SanitizeFloat(CurrentAmmo, 0) + TEXT(" / ") + FString::SanitizeFloat(MaxAmmo, 0);
					AmmoText->SetText(FText::FromString(AmmoString));
				}
			}
		}
	}
}
