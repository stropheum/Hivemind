#pragma once
#include "FoodSource.h"

class FoodSourceManager
{
public:

	/// Accessor method for the singleton
	/// @Return: A reference to the singleton instance
	static FoodSourceManager* GetInstance();

	/// Destructor
	~FoodSourceManager();

	/// Spawns a food source at the specified position and adds it to the collection of food sources
	/// @Param position: The position that the food source will be spawned at
	void SpawnFoodSource(const sf::Vector2f& position);

	/// Disseminates update calls to all food sources
	/// @Param window: The screen which all food sources are being rendered to
	/// @Param deltaTime: The time since last update call
	void Update(sf::RenderWindow& window, const float& deltaTime);

	/// Disseminates render calls to all food sources
	/// @Param window: The screen which all food sources are being rendered to
	void Render(sf::RenderWindow& window);

	/// Accessor method for the size of the food source lsit
	/// @Retrun: The total number of food sources in the simulation
	std::uint32_t GetFoodsourceCount() const;

	/// Accessor method for the begin iterator of the food source collection
	/// @Return: An iterator pointing to the beginning of the food source vector
	std::vector<FoodSource*>::iterator Begin();

	/// Accessor method for the end iterator of the food source collection
	/// @Retrun: An iterator pointing tot he end of the food source vector
	std::vector<FoodSource*>::iterator End();

	/// Accessor method for spawned food sources
	/// @Param index: The index of the food source (index corresponds to order of spawning)
	/// @Return: A reference to the food source at the specified index
	/// @Exception: Thrown if accessing an index outside of the bounds of the vector
	FoodSource& GetFoodSource(const std::uint32_t& index);

private:

	/// Constructor
	FoodSourceManager();

	static FoodSourceManager* sInstance;
	std::vector<FoodSource*> mFoodSources;
};

