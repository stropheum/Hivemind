#include "pch.h"
#include "BeeManager.h"
#include "FoodSourceManager.h"


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
	auto foodSourceManager = FoodSourceManager::getInstance();

	for (auto iter = mBees.begin(); iter != mBees.end(); ++iter)
	{
		iter->update(window, deltaTime);
		
		bool collidingWithFoodSource = false;
		for (auto foodIter = foodSourceManager->begin(); foodIter != foodSourceManager->end(); ++foodIter)
		{
			if (iter->collidingWithFoodSource(*foodIter))
			{
				collidingWithFoodSource = true;
			}
		}

		iter->setColor(collidingWithFoodSource ? Bee::ALERT_COLOR : Bee::NORMAL_COLOR);
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
