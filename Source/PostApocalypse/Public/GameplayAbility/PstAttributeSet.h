

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PstAttributeSet.generated.h"

/**
 * macro defines a set of helper functions for accessing and initializing attributes.
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class POSTAPOCALYPSE_API UPstAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UPstAttributeSet();

	UWorld* GetWorld() const override;
};
