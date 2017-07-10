#include "pch.h"
#include "CppUnitTest.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace HivemindLibraryTest
{
	TEST_CLASS(BeeTest)
	{
	public:

		map<string, const wchar_t*> Message =
		{
			{"Stub", L"Test Not Yet Implemented"},
			{"Collision", L"Invalid Collision Detection"},
			{"Nullptr Expected", L"Method was expecting to return nullptr but did not"},
			{"Default Expected", L"Method was expecting to return a default value but did not"},
		};

		static void InitializeLeakDetection()
		{
#if _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
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
#endif
		}

		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			// Initialize entities once so static initialization does not happen during leak detection
			sf::Vector2f position(0, 0);
			Hive hive(position);
			FooBee bee(position, hive);
			FoodSource fs(position);
		}

		TEST_METHOD_INITIALIZE(MethodInitialize)
		{
			InitializeLeakDetection();
		}

		TEST_METHOD_CLEANUP(MethodCleanup)
		{
			FinalizeLeakDetection();
		}

		TEST_METHOD(Bee_Construction)
		{
			sf::Vector2f position(0, 0);
			Hive hive(position);
			FooBee bee(position, hive);
			Assert::IsTrue(true);
		}

		TEST_METHOD(Bee_Render)
		{
			sf::RenderWindow window;
			sf::Vector2f position(0, 0);
			Hive hive(position);
			FooBee bee(position, hive);
			bee.Render(window);
			Assert::IsTrue(true);
		}

		TEST_METHOD(Bee_CollidingWithFoodSource)
		{
			sf::Vector2f origin(0, 0);
			FoodSource foodSource(origin);
			Hive hive(origin);

			sf::Vector2f centerOffset = foodSource.GetDimensions() / 2.0f;
			sf::Vector2f beePosition = origin + centerOffset;
			FooBee bee(beePosition, hive);

			// Center collision
			Assert::IsTrue(bee.CollidingWithFoodSource(foodSource), Message["Collision"]);

			// Centered on origin
			bee.SetPosition(origin);
			Assert::IsTrue(bee.CollidingWithFoodSource(foodSource), Message["Collision"]);

			// Centered on edge corner
			bee.SetPosition(origin + foodSource.GetDimensions());
			Assert::IsTrue(bee.CollidingWithFoodSource(foodSource), Message["Collision"]);

			// Barely touching origin
			bee.SetPosition(sf::Vector2f(0 - (bee.BODY_RADIUS - 1), 0 - (bee.BODY_RADIUS - 1)));
			Assert::IsTrue(bee.CollidingWithFoodSource(foodSource), Message["Collision"]);

			// Barely touching edge corner
			auto bottomRight = foodSource.GetDimensions();
			bottomRight.x += (bee.BODY_RADIUS - 1);
			bottomRight.y += (bee.BODY_RADIUS - 1);
			bee.SetPosition(bottomRight);
			Assert::IsTrue(bee.CollidingWithFoodSource(foodSource), Message["Collision"]);

			// Not colliding
			foodSource.SetPosition(sf::Vector2f(500, 500));
			Assert::IsFalse(bee.CollidingWithFoodSource(foodSource), Message["Collision"]);
		}

		TEST_METHOD(Bee_CollidingWithHive)
		{
			sf::Vector2f origin(0, 0);
			Hive hive(origin);

			sf::Vector2f centerOffset = hive.GetDimensions() / 2.0f;
			sf::Vector2f beePosition = origin + centerOffset;
			FooBee bee(beePosition, hive);

			// Center collision
			Assert::IsTrue(bee.CollidingWithHive(hive), Message["Collision"]);

			// Centered on origin
			bee.SetPosition(origin);
			Assert::IsTrue(bee.CollidingWithHive(hive), Message["Collision"]);

			// Centered on edge corner
			bee.SetPosition(origin + hive.GetDimensions());
			Assert::IsTrue(bee.CollidingWithHive(hive), Message["Collision"]);

			// Barely touching origin
			bee.SetPosition(sf::Vector2f(0 - (bee.BODY_RADIUS - 1), 0 - (bee.BODY_RADIUS - 1)));
			Assert::IsTrue(bee.CollidingWithHive(hive), Message["Collision"]);

			// Barely touching edge corner
			auto bottomRight = hive.GetDimensions();
			bottomRight.x += (bee.BODY_RADIUS - 1);
			bottomRight.y += (bee.BODY_RADIUS - 1);
			bee.SetPosition(bottomRight);
			Assert::IsTrue(bee.CollidingWithHive(hive), Message["Collision"]);

			// Not colliding
			hive.SetPosition(sf::Vector2f(500, 500));
			Assert::IsFalse(bee.CollidingWithHive(hive), Message["Collision"]);
		}

		TEST_METHOD(Bee_Target)
		{
			sf::Vector2f position(0, 0);
			Hive hive(position);
			FooBee bee(position, hive);

			Assert::AreEqual(sf::Vector2f(), bee.GetTarget(), Message["Default Expected"]);

			sf::Vector2f target(100, 100);
			Assert::AreNotEqual(target, bee.GetTarget());

			bee.SetTarget(target);
			Assert::AreEqual(target, bee.GetTarget());
		}

		TEST_METHOD(Bee_HasTarget)
		{
			sf::Vector2f position(0, 0);
			Hive hive(position);
			FooBee bee(position, hive);

			Assert::IsFalse(bee.HasTarget());

			bee.SetTarget(position);
			Assert::IsTrue(bee.HasTarget());
		}

		TEST_METHOD(Bee_HarvestAndDepositFood)
		{
			sf::Vector2f position(0, 0);
			Hive hive(position);
			FooBee bee(position, hive);

			Assert::AreEqual(0.0f, bee.GetFoodAmount());

			bee.HarvestFood(10.0f);
			Assert::AreNotEqual(0.0f, bee.GetFoodAmount());
			Assert::AreEqual(10.0f, bee.GetFoodAmount());
			Assert::AreEqual(0.0f, hive.GetFoodAmount());

			bee.DepositFood(6.0f);
			Assert::AreEqual(4.0f, bee.GetFoodAmount());
			Assert::AreEqual(6.0f, hive.GetFoodAmount());

			bee.DepositFood(6.0f);
			Assert::AreEqual(0.0f, bee.GetFoodAmount());
			Assert::AreEqual(10.0f, hive.GetFoodAmount());
		}

		TEST_METHOD(Bee_State)
		{
			sf::Vector2f position(0, 0);
			Hive hive(position);
			FooBee bee(position, hive);

			Assert::AreEqual(Bee::State::SeekingTarget, bee.GetState());

			bee.SetState(Bee::State::DeliveringFood);
			Assert::AreEqual(Bee::State::DeliveringFood, bee.GetState());

			bee.SetState(Bee::State::SeekingTarget);
			Assert::AreEqual(Bee::State::SeekingTarget, bee.GetState());

			bee.SetState(Bee::State::Idle);
			Assert::AreEqual(Bee::State::Idle, bee.GetState());

			bee.SetState(Bee::State::HarvestingFood);
			Assert::AreEqual(Bee::State::HarvestingFood, bee.GetState());

			bee.SetState(Bee::State::DepositingFood);
			Assert::AreEqual(Bee::State::DepositingFood, bee.GetState());

			bee.SetState(Bee::State::Scouting);
			Assert::AreEqual(Bee::State::Scouting, bee.GetState());

			// Testing repeated set state making sure that causes no problems
			bee.SetState(Bee::State::Scouting);
			Assert::AreEqual(Bee::State::Scouting, bee.GetState());
		}

		static _CrtMemState sStartMemState;
	};

	_CrtMemState BeeTest::sStartMemState;
}