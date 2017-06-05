#pragma once
class FoodSource;

class FoodSourceManager
{
public:
	static FoodSourceManager* getInstance();
	~FoodSourceManager();
	void spawnFoodSource(const sf::Vector2f& position);
	void update(sf::RenderWindow& window, const float& deltaTime);
	void render(sf::RenderWindow& window);
	std::vector<FoodSource>::iterator begin();
	std::vector<FoodSource>::iterator end();

private:
	FoodSourceManager();
	static FoodSourceManager* sInstance;
	std::vector<FoodSource> mFoodSources;
};

