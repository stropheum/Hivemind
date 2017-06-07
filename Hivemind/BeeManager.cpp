#include "pch.h"
#include "BeeManager.h"
#include "FoodSourceManager.h"
#include <random>
#include "Entity.h"


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
	mBees.clear();
}

void BeeManager::spawnBee(const sf::Vector2f& position)
{
	mBees.push_back(Bee(position));
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
			if (!iter->hasTarget())
			{	// Set initial target
				std::uniform_int_distribution<int> distribution(0, foodSourceManager->getFoodsourceCount() - 1);
				int targetIndex = distribution(generator);
				sf::Vector2f newTarget = foodSourceManager->getFoodSource(targetIndex).getCenterTarget();

				iter->setTarget(newTarget);
			}

			if (iter->collidingWithFoodSource(*foodIter))
			{
				collidingWithFoodSource = true;
				if (Entity::distanceBetween(iter->getPosition(), iter->getTarget()) <= Bee::BODY_RADIUS)
				{
					sf::Vector2f newTarget;
					do
					{
						std::uniform_int_distribution<int> distribution(0, foodSourceManager->getFoodsourceCount() - 1);
						int targetIndex = distribution(generator);
						newTarget = foodSourceManager->getFoodSource(targetIndex).getCenterTarget();
					} while (newTarget == iter->getTarget());

					iter->setTarget(newTarget);
				}
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
