


#include "GameplayAbility/PstAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstAttributeSet)

UPstAttributeSet::UPstAttributeSet()
{

}

UWorld* UPstAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}
