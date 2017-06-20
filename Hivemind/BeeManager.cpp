#include "pch.h"
#include "BeeManager.h"
#include <random>


BeeManager* BeeManager::sInstance = nullptr;

BeeManager::BeeManager():
	mBees(), mTimeSinceRetarget(0.0f), generator()
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
	for (auto iter = mBees.begin(); iter != mBees.end(); ++iter)
	{
		delete (*iter);
	}
	mBees.clear();
}

void BeeManager::spawnBee(const sf::Vector2f& position, const Hive& hive)
{
	mBees.push_back(new Bee(position, hive));
}

void BeeManager::update(sf::RenderWindow& window, const float& deltaTime)
{
	for (auto iter = mBees.begin(); iter != mBees.end(); ++iter)
	{
		(*iter)->update(window, deltaTime);
	}
}

void BeeManager::render(sf::RenderWindow& window)
{
	for (auto iter = mBees.begin(); iter != mBees.end(); ++iter)
	{
		(*iter)->render(window);
	}
}

std::vector<Bee*>::iterator BeeManager::begin()
{
	return mBees.begin();
}

std::vector<Bee*>::iterator BeeManager::end()
{
	return mBees.end();
}
