#pragma once
#include <rapidjson/document.h>

/**
*	@Author: Dale Diaz
*	@Date: 7/25/2017
*/

class WorldGenerator
{
public:

	/**
	 * Singleton accessor
	 * @Return: A pointer to the singleton instance
	 */
	static WorldGenerator* GetInstance();

	/**
	 * Destructor
	 */
	~WorldGenerator();

	/**
	 * Generates the world based on the json file provided
	 * @Param path: The path of the json file containing the world data
	 */
	void Generate(const std::string& path);

private:

	/**
	 * Singleton Instance
	 */
	static WorldGenerator* sInstance;

	/**
	 * Given a json array of hives, spawn them via the data manager
	 * @Param hives: Array of data objects
	 */
	void GenerateHives();

	/**
	 * Given a json array of food sources, spawn them via the food source manager
	 * @Param foodSources: Array of food source data objects
	 */
	void GenerateFoodSources();

	/**
	 * Given a json array of bees, spawn them via the bee manager
	 * @Param data: The data that the bees will belong to
	 */
	void GenerateBees(const rapidjson::Value& data, Hive& hive) const;

	/**
	 * Constructor
	 */
	WorldGenerator();

	/**
	 * Determines if the proposed spawn location will collide with the specified food source
	 * @Param spawnLocation: The spawn location in question
	 * @Param foodSource: The food source in question
	 * @Return: True if the spawn location will not collide with the food source
	 */
	bool IsValidLocation(const sf::Vector2f& spawnLocation, const FoodSource& foodSource) const;

	/**
	* Determines if the proposed spawn location will collide with the specified hive
	* @Param spawnLocation: The spawn location in question
	* @Param hive: The hive in question
	* @Return: True if the spawn location will not collide with the hive
	*/
	bool IsValidLocation(const sf::Vector2f& spawnLocation, const Hive& hive) const;


	/**
	 * Base-level data object for the world data
	 */
	rapidjson::Document mData;
	std::default_random_engine mGenerator;
};

