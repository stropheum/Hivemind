#include "pch.h"
#include "BeeManager.h"
#include <random>
#include "OnlookerBee.h"
#include "EmployedBee.h"


BeeManager* BeeManager::sInstance = nullptr;

BeeManager::BeeManager():
	mOnlookers(), mEmployees(), mTimeSinceRetarget(0.0f), generator()
{
}

BeeManager* BeeManager::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new BeeManager();
	}
	return sInstance;
}

BeeManager::~BeeManager()
{
	for (auto iter = mOnlookers.begin(); iter != mOnlookers.end(); ++iter)
	{
		delete (*iter);
	}
	for (auto iter = mEmployees.begin(); iter != mEmployees.end(); ++iter)
	{
		delete (*iter);
	}
	mOnlookers.clear();
	mEmployees.clear();
}

void BeeManager::spawnOnlooker(const sf::Vector2f& position, Hive& hive)
{
	mOnlookers.push_back(new OnlookerBee(position, hive));
}

void BeeManager::spawnEmployee(const sf::Vector2f& position, Hive& hive)
{
	mEmployees.push_back(new EmployedBee(position, hive));
}

void BeeManager::update(sf::RenderWindow& window, const float& deltaTime)
{
	for (auto iter = mOnlookers.begin(); iter != mOnlookers.end(); ++iter)
	{
		(*iter)->update(window, deltaTime);
	}
	for (auto iter = mEmployees.begin(); iter != mEmployees.end(); ++iter)
	{
		(*iter)->update(window, deltaTime);
	}
}

void BeeManager::render(sf::RenderWindow& window)
{
	for (auto iter = mOnlookers.begin(); iter != mOnlookers.end(); ++iter)
	{
		(*iter)->render(window);
	}
	for (auto iter = mEmployees.begin(); iter != mEmployees.end(); ++iter)
	{
		(*iter)->render(window);
	}
}

std::vector<OnlookerBee*>::iterator BeeManager::onlookerBegin()
{
	return mOnlookers.begin();
}

std::vector<OnlookerBee*>::iterator BeeManager::onlookerEnd()
{
	return mOnlookers.end();
}

std::vector<EmployedBee*>::iterator BeeManager::employeeBegin()
{
	return mEmployees.begin();
}

std::vector<EmployedBee*>::iterator BeeManager::employeeEnd()
{
	return mEmployees.end();
}

std::uint32_t BeeManager::onlookerCount() const
{
	return static_cast<std::uint32_t>(mOnlookers.size());
}

std::uint32_t BeeManager::employeeCount() const
{
	return static_cast<std::uint32_t>(mEmployees.size());
}

void BeeManager::toggleEmployeeFlowFields()
{
	for (auto iter = mEmployees.begin(); iter != mEmployees.end(); ++iter)
	{
		(*iter)->toggleFlowField();
	}
}

void BeeManager::setEmployeeFlowFieldOctaveCount(const std::uint32_t& octaveCount)
{
	for (auto iter = mEmployees.begin(); iter != mEmployees.end(); ++iter)
	{
		(*iter)->setFlowFieldOctaveCount(octaveCount);
	}
}
