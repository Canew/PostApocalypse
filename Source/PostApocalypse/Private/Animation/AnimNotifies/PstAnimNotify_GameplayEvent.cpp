


#include "Animation/AnimNotifies/PstAnimNotify_GameplayEvent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PstAnimNotify_GameplayEvent)

FString UPstAnimNotify_GameplayEvent::GetNotifyName_Implementation() const
{
	return TEXT("GameplayEvent");
}

void UPstAnimNotify_GameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner()))
	{
		FGameplayEventData GameplayEventData;
		GameplayEventData.ContextHandle = ASC->MakeEffectContext();
		ASC->HandleGameplayEvent(GameplayTag, &GameplayEventData);
	}
}

