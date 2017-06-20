#pragma once
#include "Bee.h"
#include <random>

class BeeManager
{
public:
	static BeeManager* getInstance();
	~BeeManager();
	void spawnBee(const sf::Vector2f& position, const Hive& hive);
	void update(sf::RenderWindow& window, const float& deltaTime);
	void render(sf::RenderWindow& window);
	std::vector<Bee*>::iterator begin();
	std::vector<Bee*>::iterator end();

private:
	BeeManager();
	static BeeManager* sInstance;
	std::vector<Bee*> mBees;
	const float FOOD_RETARGET_INTERVAL = 20.0f;
	float mTimeSinceRetarget;
	std::default_random_engine generator;
};

