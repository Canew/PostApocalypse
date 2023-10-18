


#include "PstGameplayTags.h"
#include "Engine/EngineTypes.h"

namespace PstGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move");
	UE_DEFINE_GAMEPLAY_TAG(Input_Look, "Input.Look");
	UE_DEFINE_GAMEPLAY_TAG(Input_Jump, "Input.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Input_Climb, "Input.Climb");
	UE_DEFINE_GAMEPLAY_TAG(Input_Crouch, "Input.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(Input_Sprint, "Input.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(Input_AimDownSights, "Input.AimDownSights");
	UE_DEFINE_GAMEPLAY_TAG(Input_Dash_Forward, "Input.Dash.Forward");
	UE_DEFINE_GAMEPLAY_TAG(Input_Dash_Backward, "Input.Dash.Backward");
	UE_DEFINE_GAMEPLAY_TAG(Input_Dash_Left, "Input.Dash.Left");
	UE_DEFINE_GAMEPLAY_TAG(Input_Dash_Right, "Input.Dash.Right");
	UE_DEFINE_GAMEPLAY_TAG(Input_PrimaryAction, "Input.PrimaryAction");
	UE_DEFINE_GAMEPLAY_TAG(Input_SecondaryAction, "Input.SecondaryAction");
	UE_DEFINE_GAMEPLAY_TAG(Input_Skill, "Input.Skill");
	UE_DEFINE_GAMEPLAY_TAG(Input_Reload, "Input.Reload");

	UE_DEFINE_GAMEPLAY_TAG(Gameplay_AbilityInputBlocked, "Gameplay.AbilityInputBlocked");

	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_BeginCollision, "GameplayEvent.BeginCollision");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_EndCollision, "GameplayEvent.EndCollision");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Launch, "GameplayEvent.Launch");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Reload, "GameplayEvent.Reload");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Death, "GameplayEvent.Death");

	UE_DEFINE_GAMEPLAY_TAG(Status_Death, "Status.Death");
	UE_DEFINE_GAMEPLAY_TAG(Status_Death_Dying, "Status.Death.Dying");
	UE_DEFINE_GAMEPLAY_TAG(Status_Death_Dead, "Status.Death.Dead");

	UE_DEFINE_GAMEPLAY_TAG(Data_Damage, "Data.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Data_Healing, "Data.Healing");
}
