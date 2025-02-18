// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RoomEscapeFPS : ModuleRules
{
	public RoomEscapeFPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
        { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "HeadMountedDisplay", "UMG"});
        PrivateDependencyModuleNames.AddRange(new string[]{ "Slate", "SlateCore", "Paper2D"});

    }
}
