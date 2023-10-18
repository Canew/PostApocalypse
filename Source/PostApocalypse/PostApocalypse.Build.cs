// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PostApocalypse : ModuleRules
{
	public PostApocalypse(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule" });
		PrivateDependencyModuleNames.AddRange(new string[] { "NavigationSystem" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		PrivateDependencyModuleNames.AddRange(new string[] { "UMG" });
		PrivateDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });
	}
}
