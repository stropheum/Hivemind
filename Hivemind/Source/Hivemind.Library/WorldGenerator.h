#pragma once
#include <rapidjson/document.h>

class WorldGenerator
{
public:

	/// Singleton accessor
	/// @Return: A pointer to the singleton instance
	static WorldGenerator* GetInstance();

	/// Destructor
	~WorldGenerator();

	/// Generates the world based on the json file provided
	/// @Param path: The path of the json file containing the world data 
	void Generate(const std::string& path);

private:

	/// Singleton instance
	static WorldGenerator* sInstance;

	/// Given a json array of hives, spawn them via the data manager
	/// @Param hives: Array of data data objects
	void GenerateHives();

	/// Given a json array of food sources, spawn them via the food source manager
	/// @Param foodSources: Array of food source data objects
	void GenerateFoodSources();

	/// Given a json array of bees, spawn them via the bee manager
	/// @Param data: The data that the bees will belong to
	void GenerateBees(const rapidjson::Value& data, Hive& hive);

	/// Constructor
	WorldGenerator();

	/// Base-level data object for the world data
	rapidjson::Document mData;
	std::default_random_engine mGenerator;
};

