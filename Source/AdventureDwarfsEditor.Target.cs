// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AdventureDwarfsEditorTarget : TargetRules
{
	public AdventureDwarfsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		//IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;
        		bOverrideBuildEnvironment = true;

		ExtraModuleNames.Add("AdventureDwarfs");
	}
}
