#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HivemindLibraryTest
{		
	TEST_CLASS(EntityTest)
	{
	public:

		static void InitializeLeakDetection()
		{
#if _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif _DEBUG
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

		TEST_METHOD_INITIALIZE(MethodInitialize)
		{
			InitializeLeakDetection();
		}

		TEST_METHOD_CLEANUP(MethodCleanup)
		{
			FinalizeLeakDetection();
		}
		
		TEST_METHOD(Entity_Construction)
		{
			sf::Vector2f position(100, 100);
			FooEntity entity(position);
			Assert::AreEqual(sf::Vector2f(100, 100), entity.GetPosition());
		}

		TEST_METHOD(Entity_Position)
		{
			sf::Vector2f position(100, 100);
			FooEntity entity(position);
			Assert::AreEqual(position, entity.GetPosition());

			sf::Vector2f newPosition(500, 500);
			entity.SetPosition(newPosition);
			Assert::AreEqual(newPosition, entity.GetPosition());
			Assert::AreNotEqual(position, entity.GetPosition());
		}

		static _CrtMemState sStartMemState;
	};
	
	_CrtMemState EntityTest::sStartMemState;
}