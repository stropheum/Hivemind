#pragma once
#include "FoodSource.h"

class FoodSourceManager
{
public:
	static FoodSourceManager* GetInstance();
	~FoodSourceManager();
	void SpawnFoodSource(const sf::Vector2f& position);
	void Update(sf::RenderWindow& window, const float& deltaTime);
	void Render(sf::RenderWindow& window);
	std::uint32_t GetFoodsourceCount() const;
	std::vector<FoodSource*>::iterator Begin();
	std::vector<FoodSource*>::iterator End();
	std::uint32_t FoodSourceCount() const;
	FoodSource& GetFoodSource(const std::uint32_t& index);

private:
	FoodSourceManager();
	static FoodSourceManager* sInstance;
	std::vector<FoodSource*> mFoodSources;
};

