#pragma once
#include "Bee.h"
#include <random>

class BeeManager
{
public:
	static BeeManager* getInstance();
	~BeeManager();
	void spawnOnlooker(const sf::Vector2f& position, Hive& hive);
	void spawnEmployee(const sf::Vector2f& position, Hive& hive);
	void update(sf::RenderWindow& window, const float& deltaTime);
	void render(sf::RenderWindow& window);
	std::vector<class OnlookerBee*>::iterator onlookerBegin();
	std::vector<class OnlookerBee*>::iterator onlookerEnd();
	std::vector<class EmployedBee*>::iterator employeeBegin();
	std::vector<class EmployedBee*>::iterator employeeEnd();
	std::uint32_t onlookerCount() const;
	std::uint32_t employeeCount() const;

private:
	BeeManager();
	static BeeManager* sInstance;
	std::vector<class OnlookerBee*> mOnlookers;
	std::vector<class EmployedBee*> mEmployees;
	const float FOOD_RETARGET_INTERVAL = 20.0f;
	float mTimeSinceRetarget;
	std::default_random_engine generator;
};

