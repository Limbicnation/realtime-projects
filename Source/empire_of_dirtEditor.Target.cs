// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class empire_of_dirtEditorTarget : TargetRules
{
	public empire_of_dirtEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

		ExtraModuleNames.AddRange( new string[] { "empire_of_dirt" } );

		// Explicitly set bStrictConformanceMode
		bOverrideBuildEnvironment = true;
		WindowsPlatform.bStrictConformanceMode = true;
	}
}
