


#include "Character/PstCharacterSetting.h"
#include "GameplayAbility/PstAbilitySet.h"
#include "Input/PstInputConfig.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstCharacterSetting)

UPstCharacterSetting::UPstCharacterSetting(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CharacterClass = nullptr;
	InputConfig = nullptr;
}
