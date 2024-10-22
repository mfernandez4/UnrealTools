// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UEditorExtension : ModuleRules
{
	public UEditorExtension(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// System.IO.Path.GetFullPath(Target.RelativeEnginePath) is the path to the engine folder
				// "/Source/Editor/Blutility/Private" allows access to the private folder of the Blutility module
				System.IO.Path.GetFullPath(Target.RelativeEnginePath) + "/Source/Editor/Blutility/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Blutility",
				"EditorScriptingUtilities",
				"Niagara",
				"UMG",
				"UnrealEd",
				"AssetTools",
				"ContentBrowser",
				"EditorStyle",
				"InputCore",
				"SlateCore",
				"LevelEditor",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"LevelEditor",
				"Projects",
				// "InputCore",
				// "UnrealEd",

				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
