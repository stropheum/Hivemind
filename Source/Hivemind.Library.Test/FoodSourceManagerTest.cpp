#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HivemindLibraryTest
{
	TEST_CLASS(FoodSourceManagerTest)
	{
	public:

		static void InitializeLeakDetection()
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		/// Detects if memory state has been corrupted
		static void FinalizeLeakDetection()
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}

			// ReSharper disable CppSomeObjectMembersMightNotBeInitialized
			UNREFERENCED_PARAMETER(endMemState);
			UNREFERENCED_PARAMETER(diffMemState);
			// ReSharper restore CppSomeObjectMembersMightNotBeInitialized
		}

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			FoodSourceManager::GetInstance(); // Lazily load singleton to not trigger leak detection
		}

		TEST_METHOD_INITIALIZE(MethodInitialize)
		{
			InitializeLeakDetection();
		}

		TEST_METHOD_CLEANUP(MethodCleanup)
		{
			FinalizeLeakDetection();
		}

		TEST_METHOD(FoodSourceManager_Stub)
		{
			auto foodSourceManager = FoodSourceManager::GetInstance();
			UNREFERENCED_PARAMETER(foodSourceManager);
			Assert::IsTrue(true, L"Test not implemented");
		}

		static _CrtMemState sStartMemState;
	};

	_CrtMemState FoodSourceManagerTest::sStartMemState;
}