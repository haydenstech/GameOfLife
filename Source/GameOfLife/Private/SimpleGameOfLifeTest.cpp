#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SimpleGameOfLifeTest, "GameOfLife.GameOfLife.Private.SimpleGameOfLifeTest",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

//Tests can't be in CPP only modules... should create a test module that's only present in editor target.
bool SimpleGameOfLifeTest::RunTest(const FString& Parameters)
{
	// Make the test pass by returning true, or fail by returning false.
	UE_LOG(LogTemp, Display, TEXT("Hello from simple test"));
	return true;
}
