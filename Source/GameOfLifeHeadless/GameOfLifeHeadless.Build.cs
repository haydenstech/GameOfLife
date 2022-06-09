using UnrealBuildTool;
using System.IO;

public class GameOfLifeHeadless : ModuleRules
{
	public GameOfLifeHeadless(ReadOnlyTargetRules Target) : base(Target)
	{
		bRequiresImplementModule = false;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"ApplicationCore",
				"Projects",
				// ---
				"GameOfLifeCore"
			});

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"Launch",
			});
	}
}