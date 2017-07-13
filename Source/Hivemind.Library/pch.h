#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


//////////////////////////////
//  Program Dependencies  ///
////////////////////////////
#include <windows.h>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <math.h>
#include <sstream>
#include <fstream>
#include <rapidjson/document.h>


///////////////////////////
//  Local Dependencies  //
/////////////////////////
#include "BeeManager.h"
#include "FoodSourceManager.h"
#include "FlowFieldManager.h"
#include "FlowField.h"
#include "FontManager.h"
#include "HiveManager.h"
#include "Hive.h"
#include "FoodSource.h"
#include "OnlookerBee.h"
#include "EmployedBee.h"
#include "QueenBee.h"
#include "Drone.h"
#include "Guard.h"
#include "Larva.h"
#include "CollisionNode.h"
#include "CollisionGrid.h"
