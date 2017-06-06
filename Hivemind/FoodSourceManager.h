#pragma once
#include "FoodSource.h"

class FoodSourceManager
{
public:
	static FoodSourceManager* getInstance();
	~FoodSourceManager();
	void spawnFoodSource(const sf::Vector2f& position);
	void update(sf::RenderWindow& window, const float& deltaTime);
	void render(sf::RenderWindow& window);
	std::uint32_t getFoodsourceCount() const;
	std::vector<FoodSource>::iterator begin();
	std::vector<FoodSource>::iterator end();
	const FoodSource& getFoodSource(const std::uint32_t& index);

private:
	FoodSourceManager();
	static FoodSourceManager* sInstance;
	std::vector<FoodSource> mFoodSources;
};

