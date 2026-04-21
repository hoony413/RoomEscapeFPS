// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RoomEscapeFPS : ModuleRules
{
	public RoomEscapeFPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;

		PublicDependencyModuleNames.AddRange(new string[]
        { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "UMG", "AssetRegistry", "CommonUI", "CommonInput", "DeveloperSettings", "ModelViewViewModel", "EnhancedInput", "Niagara"});
        PrivateDependencyModuleNames.AddRange(new string[]{ "Slate", "SlateCore", "Paper2D"});

        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "AssetTools" });
        }
    }
}
