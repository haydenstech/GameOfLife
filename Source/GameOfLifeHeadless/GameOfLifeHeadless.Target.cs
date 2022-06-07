using UnrealBuildTool;
using System.Collections.Generic;

public class GameOfLifeHeadlessTarget : TargetRules
{
	public GameOfLifeHeadlessTarget(TargetInfo Target) : base(Target)
	{
		DefaultBuildSettings = BuildSettingsVersion.V2;
		
		Type = TargetType.Program;
		LaunchModuleName = "GameOfLifeHeadless";
		LinkType = TargetLinkType.Monolithic;

		bUseMallocProfiler = false;
		bUseLoggingInShipping = true;

		bCompileICU = false;
		bBuildWithEditorOnlyData = true;

		bCompileAgainstEngine = false;
		bCompileAgainstCoreUObject = true;
		bCompileAgainstApplicationCore = true;

		bForceEnableExceptions = false;

		bCompileWithPluginSupport = false;
		bBuildDeveloperTools = false;

		bIsBuildingConsoleApplication = true;
	}
}