// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AdventureDwarfsTarget : TargetRules
{
	public AdventureDwarfsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		//IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
        		bOverrideBuildEnvironment = true;

		ExtraModuleNames.Add("AdventureDwarfs");
	}
}
