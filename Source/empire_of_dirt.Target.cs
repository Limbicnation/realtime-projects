// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class empire_of_dirtTarget : TargetRules
{
	public empire_of_dirtTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "empire_of_dirt" } );
	}
}
