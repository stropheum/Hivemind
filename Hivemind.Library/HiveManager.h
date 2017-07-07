#pragma once
#include <random>


class Hive;

class HiveManager
{
public:

	/// Accessor method for the singleton
	/// @Return: A pointer to the singleton's instance
	static HiveManager* GetInstance();

	/// Destructor
	~HiveManager();

	/// Spawns a hive and adds it to the list of existing hives
	/// @Param position: The position of the hive in the world
	void SpawnHive(const sf::Vector2f& position);

	/// Disseminates update calls to all existing hives
	/// @Param window: The screen which all hives are rendered to
	/// @Param deltaTime: The time since last update call
	void Update(sf::RenderWindow& window, const float& deltaTime);

	/// Disseminates render calls to all existing hives
	/// @Param window: The screen which all hives are rendered to
	void Render(sf::RenderWindow& window);

	/// Accessor method for the beginning of the list of hives
	/// @Return: An iterator pointing to the beginning of the list of hives
	std::vector<Hive*>::iterator Begin();

	/// Accessor methodf or the end of the list of hives
	/// @Return: An iterator pointing ot the end of the list of hives
	std::vector<Hive*>::iterator End();

	/// Accessor method for hives based on the index of which they were added
	/// @Param index: The index of the hive being access
	/// @Exception: Thrown if accessing beyond the bounds of the vector
	Hive* GetHive(std::uint32_t index);

private:

	/// Constructor
	HiveManager();

	static HiveManager* sInstance;
	std::vector<Hive*> mHives;
	std::default_random_engine generator;
};

