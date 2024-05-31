// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AdventureDwarfs : ModuleRules
{
	public AdventureDwarfs(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay",
			"EnhancedInput",
			"Json",
			"JsonUtilities",
			"UMG"
		});
	}
}
