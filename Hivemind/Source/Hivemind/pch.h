#pragma once

#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>


//////////////////////////////
//  Program Dependencies  ///
////////////////////////////
#include <windows.h>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <chrono>
#include <sstream>
#include "BeeManager.h"
#include "FoodSourceManager.h"
#include "HiveManager.h"
#include "Hive.h"
#include "FlowField.h"
#include "WorldGenerator.h"
#include "FlowFieldManager.h"
#include "CollisionNode.h"
#include "CollisionGrid.h"
#include "Wasp.h"
#include "WaspManager.h"