#pragma once

#include <SDKDDKVer.h>
// Headers for CppUnitTest
#include "CppUnitTest.h"


/////////////////////////////
//  Project Dependencies  //
///////////////////////////
#include <windows.h>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>


///////////////////////////
//  Local Dependencies  //
/////////////////////////
#include "Entity.h"
#include "FooEntity.h"
#include "Bee.h"
#include "FooBee.h"
#include "BeeManager.h"
#include "FoodSourceManager.h"
#include "HiveManager.h"
#include "Hive.h"
#include "FoodSource.h"
#include "OnlookerBee.h"
#include "EmployedBee.h"
#include "QueenBee.h"
#include "Drone.h"
#include "Guard.h"
#include "Larva.h"


/////////////////////////////////
//  Template Specializations  //
///////////////////////////////
namespace Microsoft {
	namespace VisualStudio {
		namespace CppUnitTestFramework {

			template<> inline std::wstring ToString<sf::Vector2f>(const sf::Vector2f& vector)
			{
				std::stringstream ss;
				ss << vector.x << ", " << vector.y;
				auto string = ss.str();
				std::wstring result(string.length(), L' ');
				std::copy(string.begin(), string.end(), result.begin());
				return result;
			}

			template<> inline std::wstring ToString<sf::Vector2i>(const sf::Vector2i& vector)
			{
				std::stringstream ss;
				ss << vector.x << ", " << vector.y;
				auto string = ss.str();
				std::wstring result(string.length(), L' ');
				std::copy(string.begin(), string.end(), result.begin());
				return result;
			}

			template<> inline std::wstring ToString<Bee::State>(const Bee::State& state)
			{
				auto result = L"";
				switch (state)
				{
				case Bee::Idle: 
					result = L"Idle";
					break;
				case Bee::Scouting: 
					result = L"Scouting";
					break;
				case Bee::SeekingTarget: 
					result = L"SeekingTarget";
					break;
				case Bee::HarvestingFood: 
					result = L"HarvestingFood";
					break;
				case Bee::DeliveringFood: 
					result = L"DeliveringFood";
					break;
				case Bee::DepositingFood: 
					result = L"DepositingFood";
					break;
				}

				return result;
			}

		}
	}
}