#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HivemindLibraryTest
{
	TEST_CLASS(QueenTest)
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

		TEST_METHOD_INITIALIZE(methodInitialize)
		{
			InitializeLeakDetection();
		}

		TEST_METHOD_CLEANUP(methodCleanup)
		{
			FinalizeLeakDetection();
		}

		TEST_METHOD(UnitTest1)
		{
			Assert::Fail(L"Test not implemented");
		}

		static _CrtMemState sStartMemState;
	};

	_CrtMemState QueenTest::sStartMemState;
}