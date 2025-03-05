// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SimpleDoor : ModuleRules
{
	public SimpleDoor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemNull", "OnlineSubsystemUtils" });

        PublicIncludePaths.AddRange(new string[] { "SimpleDoor", "SimpleDoor/BasicCharacter", 
			"SimpleDoor/ThirdPersonCharacter", "SimpleDoor/FirstPersonCharacter" ,"SimpleDoor/Door", 
			"SimpleDoor/UseInterface", "SimpleDoor/Bomb","SimpleDoor/GameInstance", "SimpleDoor/GameMode",
			"SimpleDoor/MenuWidgets", "SimpleDoor/ViewList"});
	}
}
