// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnrealTools : ModuleRules
{
	public UnrealTools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "Blutility" });

        PublicIncludePaths.AddRange(new string[] { System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "/Source/Editor/Blutility/Private" });
    }
}
