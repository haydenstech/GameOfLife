using UnrealBuildTool;

public class GameOfLifeCore : ModuleRules
{
	public GameOfLifeCore(ReadOnlyTargetRules Target) : base(Target)
	{
	    bRequiresImplementModule = false;

	    Type = ModuleType.CPlusPlus;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        bEnforceIWYU = true;

        PublicDependencyModuleNames.AddRange(
			new string[] {
			"Core",
			"CoreUObject", 
			"GeometricObjects",
		});
	}
}
