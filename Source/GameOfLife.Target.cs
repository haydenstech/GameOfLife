// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GameOfLifeTarget : TargetRules
{
	public GameOfLifeTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2; //Headless included here so that it's included in the generated solution file.
		ExtraModuleNames.AddRange( new string[] { "GameOfLife", "GameOfLifeHeadless" } ); 
	}
}
