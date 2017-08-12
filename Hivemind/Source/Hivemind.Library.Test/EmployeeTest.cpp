#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HivemindLibraryTest
{
	TEST_CLASS(EmployeeTest)
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
			// Make sure at least one hive/employee is created so leak detection won't pickup static initialization
			sf::Vector2f position(0, 0);
			Hive hive(position);
			EmployedBee employee(position, hive);
		}

		TEST_METHOD_INITIALIZE(MethodInitialize)
		{
			InitializeLeakDetection();
		}

		TEST_METHOD_CLEANUP(MethodCleanup)
		{
			FinalizeLeakDetection();
		}

		TEST_METHOD(Employee_Stub)
		{
			sf::Vector2f position(0, 0);
			Hive hive(position);
			EmployedBee employee(position, hive);
			Assert::IsTrue(true, L"Test not implemented");
		}

		TEST_METHOD(Employee_Update)
		{
			Assert::Fail(L"Test not implemented");
		}

		TEST_METHOD(Employee_GenerateNewTarget)
		{
			Assert::Fail(L"Test not implemented");
		}

		static _CrtMemState sStartMemState;
	};

	_CrtMemState EmployeeTest::sStartMemState;
}