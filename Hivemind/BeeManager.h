#pragma once
#include "Bee.h"
#include <random>

class BeeManager
{
public:
	static BeeManager* GetInstance();
	~BeeManager();
	void SpawnOnlooker(const sf::Vector2f& position, Hive& hive);
	void SpawnEmployee(const sf::Vector2f& position, Hive& hive);
	void Update(sf::RenderWindow& window, const float& deltaTime);
	void Render(sf::RenderWindow& window);
	std::vector<class OnlookerBee*>::iterator OnlookerBegin();
	std::vector<class OnlookerBee*>::iterator OnlookerEnd();
	std::vector<class EmployedBee*>::iterator EmployeeBegin();
	std::vector<class EmployedBee*>::iterator EmployeeEnd();
	std::uint32_t OnlookerCount() const;
	std::uint32_t EmployeeCount() const;
	void ToggleEmployeeFlowFields();
	void SetEmployeeFlowFieldOctaveCount(const std::uint32_t& octaveCount);

private:
	BeeManager();
	static BeeManager* sInstance;
	std::vector<class OnlookerBee*> mOnlookers;
	std::vector<class EmployedBee*> mEmployees;
	const float FOOD_RETARGET_INTERVAL = 20.0f;
	float mTimeSinceRetarget;
	std::default_random_engine generator;
};

