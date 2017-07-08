#include "pch.h"
#include "CppUnitTest.h"
#include "PerlinNoise.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace HivemindLibraryTest
{
	TEST_CLASS(PerlinNoiseTest)
	{
	public:

		map<string, const wchar_t*> Message =
		{
			{ "Stub", L"Test Not Yet Implemented" },
			{ "Collision", L"Invalid Collision Detection" },
			{ "Nullptr Expected", L"Method was expecting to return nullptr but did not" },
			{ "Default Expected", L"Method was expecting to return a default value but did not" },
		};

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

		TEST_METHOD_INITIALIZE(MethodInitialize)
		{
			InitializeLeakDetection();
		}

		TEST_METHOD_CLEANUP(MethodCleanup)
		{
			FinalizeLeakDetection();
		}

		TEST_METHOD(PerlinNoise_GenerateWhiteNoise)
		{
			PerlinNoise noise;
			auto map = noise.GenerateWhiteNoise(sf::Vector2i(100, 100));
			
			for (int i = 0; i < 100; i++)
			{	// Make sure every row is initialized
				Assert::AreNotEqual(static_cast<float*>(nullptr), map[i]);
				for (int j = 0; j < 100; j++)
				{	// Make sure values remain generated between 0 and 1
					Assert::IsTrue(map[i][j] >= 0.0f && map[i][j] <= 1.0f);
				}
			}

			for (int i = 0; i < 100; i++)
			{
				delete[] map[i];
			}
			delete[] map;
		}

		TEST_METHOD(PerlinNoise_GenerateSmoothNoise)
		{
			PerlinNoise noise;
			sf::Vector2i dimensions(100, 100);
			auto noiseMap = noise.GenerateWhiteNoise(dimensions);
			auto perlinMap = noise.GeneratePerlinNoise(noiseMap, dimensions, 10);
			
			for (int i = 0; i < 100; i++)
			{	// Make sure every row is initialized
				Assert::AreNotEqual(static_cast<float*>(nullptr), noiseMap[i]);
				for (int j = 0; j < 100; j++)
				{	// Make sure values remain generated between 0 and 1
					Assert::IsTrue(noiseMap[i][j] >= 0.0f && noiseMap[i][j] <= 1.0f);
				}
			}

			for (int i = 0; i < 100; i++)
			{	// Make sure every row is initialized
				Assert::AreNotEqual(static_cast<float*>(nullptr), perlinMap[i]);
				for (int j = 0; j < 100; j++)
				{	// Make sure values remain generated between 0 and 1
					Assert::IsTrue(perlinMap[i][j] >= 0.0f && perlinMap[i][j] <= 1.0f);
				}
			}

			for (int i = 0; i < 100; i++)
			{
				delete[] noiseMap[i];
			}
			delete[] noiseMap;

			for (int i = 0; i < 100; i++)
			{
				delete[] perlinMap[i];
			}
			delete[] perlinMap;
		}

		TEST_METHOD(PerlinNoise_GeneratePerlinNoise)
		{
			PerlinNoise noise;
			sf::Vector2i dimensions(100, 100);
			auto noiseMap = noise.GenerateWhiteNoise(dimensions);
			auto smoothMap = noise.GenerateSmoothNoise(noiseMap, dimensions, 1);

			for (int i = 0; i < 100; i++)
			{	// Make sure every row is initialized
				Assert::AreNotEqual(static_cast<float*>(nullptr), noiseMap[i]);
				for (int j = 0; j < 100; j++)
				{	// Make sure values remain generated between 0 and 1
					Assert::IsTrue(noiseMap[i][j] >= 0.0f && noiseMap[i][j] <= 1.0f);
				}
			}

			for (int i = 0; i < 100; i++)
			{	// Make sure every row is initialized
				Assert::AreNotEqual(static_cast<float*>(nullptr), smoothMap[i]);
				for (int j = 0; j < 100; j++)
				{	// Make sure values remain generated between 0 and 1
					Assert::IsTrue(smoothMap[i][j] >= 0.0f && smoothMap[i][j] <= 1.0f);
				}
			}

			for (int i = 0; i < 100; i++)
			{
				delete[] noiseMap[i];
			}
			delete[] noiseMap;

			for (int i = 0; i < 100; i++)
			{
				delete[] smoothMap[i];
			}
			delete[] smoothMap;
		}

		static _CrtMemState sStartMemState;
	};

	_CrtMemState PerlinNoiseTest::sStartMemState;
}