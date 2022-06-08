using UnrealBuildTool;

public class GameOfLifeTests : ModuleRules
{
	public GameOfLifeTests(ReadOnlyTargetRules Target) : base(Target)
	{
		bRequiresImplementModule = false;

		Type = ModuleType.CPlusPlus;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		bEnforceIWYU = true;

		PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"GameOfLifeCore"
			});
	}
}