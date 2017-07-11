#include "pch.h"
#include "WorldGenerator.h"


using namespace std;
using namespace rapidjson;

WorldGenerator* WorldGenerator::sInstance = nullptr;

WorldGenerator::WorldGenerator():
	mData()
{
	std::random_device device;
	mGenerator = std::default_random_engine(device());
}

WorldGenerator* WorldGenerator::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new WorldGenerator();
	}
	return sInstance;
}

WorldGenerator::~WorldGenerator()
{
	if (sInstance != nullptr)
	{
		delete sInstance;
	}
}

void WorldGenerator::Generate(const std::string& path)
{
	string json;
	ifstream jsonFile(path, ifstream::binary);
	while (!jsonFile.eof())
	{
		string token;
		jsonFile >> token;
		json += token;
		
	}
	jsonFile.close();

	mData.Parse(json.c_str());

	GenerateHives();
	GenerateFoodSources();
}

void WorldGenerator::GenerateHives()
{
	if (!mData.HasMember("Hives"))
	{
		return;
	}

	assert(mData["Hives"].IsArray());
	auto& hives = mData["Hives"];
	auto hiveManager = HiveManager::GetInstance();
	
	for (uint32_t i = 0; i < hives.Size(); i++)
	{	// Construct hive data and pass it to hive manager spawn call
		assert(hives[i].HasMember("position"));
		auto& pos = hives[i]["position"];
		assert(pos.HasMember("x") && pos.HasMember("y"));
		
		auto& spawnedHive = hiveManager->SpawnHive(sf::Vector2f(pos["x"].GetDouble(), pos["y"].GetDouble()));
		GenerateBees(hives[i], spawnedHive);
	}
}

void WorldGenerator::GenerateFoodSources()
{
	if (!mData.HasMember("FoodSources"))
	{
		return;
	}

	assert(mData["FoodSources"].IsArray());
	auto& foodSources = mData["FoodSources"];
	auto foodSourceManager = FoodSourceManager::GetInstance();

	for (uint32_t i = 0; i < foodSources.Size(); i++)
	{	// Construct food source data and pass it to the food source manager
		if (foodSources[i].HasMember("position"))
		{
			auto& pos = foodSources[i]["position"];
			assert(pos.HasMember("x") && pos.HasMember("y"));
			foodSourceManager->SpawnFoodSource(sf::Vector2f(pos["x"].GetDouble(), pos["y"].GetDouble()));
		}

		else if (foodSources[i].HasMember("random_position"))
		{
			auto& pos = foodSources[i]["random_position"];
			assert(pos.HasMember("lower_bound"));
			assert(pos.HasMember("upper_bound"));

			auto& lower = pos["lower_bound"];
			auto& upper = pos["upper_bound"];
			assert(lower.IsDouble());
			assert(upper.IsDouble());

			std::uniform_real_distribution<float> distribution(lower.GetDouble(), upper.GetDouble());
			foodSourceManager->SpawnFoodSource(sf::Vector2f(distribution(mGenerator), distribution(mGenerator)));
		}
	}
}

void WorldGenerator::GenerateBees(const rapidjson::Value& data, Hive& hive)
{
	auto beeManager = BeeManager::GetInstance();
	auto spawnLocation = hive.GetCenterTarget();

	beeManager->SpawnQueen(spawnLocation, hive);

	if (data.HasMember("Onlookers"))
	{
		for (int i = 0; i < data["Onlookers"].GetInt(); i++)
		{
			beeManager->SpawnOnlooker(spawnLocation, hive);
		}
	}

	if (data.HasMember("Employees"))
	{
		for (int i = 0; i < data["Employees"].GetInt(); i++)
		{
			beeManager->SpawnEmployee(spawnLocation, hive);
		}
	}

	if (data.HasMember("Drones"))
	{
		for (int i = 0; i < data["Drones"].GetInt(); i++)
		{
			beeManager->SpawnDrone(spawnLocation, hive);
		}
	}

	if (data.HasMember("Guards"))
	{
		for (int i = 0; i < data["Guards"].GetInt(); i++)
		{
			beeManager->SpawnGuard(spawnLocation, hive);
		}
	}
}
