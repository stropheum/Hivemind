#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HivemindLibraryTest
{
	TEST_CLASS(DroneTest)
	{
	public:

		static void InitializeLeakDetection()
		{
#if _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif //_DEBUG
		}

		/// Detects if memory state has been corrupted
		static void FinalizeLeakDetection()
		{
#if _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif //_DEBUG
		}

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			// Make sure at least one hive/drone is created so leak detection won't pickup static initialization
			sf::Vector2f position(0, 0);
			Hive hive(position);
			Drone drone(position, hive);
		}

		TEST_METHOD_INITIALIZE(MethodInitialize)
		{
			InitializeLeakDetection();
		}

		TEST_METHOD_CLEANUP(MethodCleanup)
		{
			FinalizeLeakDetection();
		}

		TEST_METHOD(Drone_Stub)
		{
			sf::Vector2f position(0, 0);
			Hive hive(position);
			Drone drone(position, hive);
			Assert::IsTrue(true, L"Test not implemented");
		}

		TEST_METHOD(Drone_Update)
		{
			Assert::Fail(L"Test not implemented");
		}

		TEST_METHOD(Drone_HandleCombManagement)
		{
			Assert::Fail(L"Test not implemented");
		}

		static _CrtMemState sStartMemState;
	};

	_CrtMemState DroneTest::sStartMemState;
}