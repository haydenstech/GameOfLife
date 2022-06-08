#include "Misc/AutomationTest.h"
#include "GameOfLifeCore.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameOfLifeLoadingTest, "GameOfLife.GameOfLifeTests.GameOfLifeLoadingTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GameOfLifeLoadingTest::RunTest(const FString& Parameters)
{
	
	// Make the test pass by returning true, or fail by returning false.
	return true;
}
