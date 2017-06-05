#include "pch.h"
#include "BeeManager.h"


BeeManager* BeeManager::sInstance = nullptr;

BeeManager::BeeManager():
	mBees()
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
	mBees.clear();
}

void BeeManager::spawnBee(const Bee& bee)
{
	mBees.push_back(bee);
}

void BeeManager::update(sf::RenderWindow& window, const float& deltaTime)
{
	for (auto iter = mBees.begin(); iter != mBees.end(); ++iter)
	{
		iter->update(window, deltaTime);
	}
}

void BeeManager::render(sf::RenderWindow& window)
{
	for (auto iter = mBees.begin(); iter != mBees.end(); ++iter)
	{
		iter->render(window);
	}
}

std::vector<Bee>::iterator BeeManager::begin()
{
	return mBees.begin();
}

std::vector<Bee>::iterator BeeManager::end()
{
	return mBees.end();
}
