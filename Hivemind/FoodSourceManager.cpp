#include "pch.h"
#include "FoodSourceManager.h"
#include "FoodSource.h"


FoodSourceManager* FoodSourceManager::sInstance = nullptr;

FoodSourceManager::FoodSourceManager()
{
}

FoodSourceManager* FoodSourceManager::getInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new FoodSourceManager();
	}
	return sInstance;
}

FoodSourceManager::~FoodSourceManager()
{
	for (auto iter = mFoodSources.begin(); iter != mFoodSources.end(); ++iter)
	{
		delete (*iter);
	}
	mFoodSources.clear();
}

void FoodSourceManager::spawnFoodSource(const sf::Vector2f& position)
{
	mFoodSources.push_back(new FoodSource(position));
}

void FoodSourceManager::update(sf::RenderWindow& window, const float& deltaTime)
{
	for (auto iter = mFoodSources.begin(); iter != mFoodSources.end(); ++iter)
	{
		(*iter)->update(window, deltaTime);
	}
}

void FoodSourceManager::render(sf::RenderWindow& window)
{
	for (auto iter = mFoodSources.begin(); iter != mFoodSources.end(); ++iter)
	{
		(*iter)->render(window);
	}
}

std::uint32_t FoodSourceManager::getFoodsourceCount() const
{
	return static_cast<std::uint32_t>(mFoodSources.size());
}

std::vector<FoodSource*>::iterator FoodSourceManager::begin()
{
	return mFoodSources.begin();
}

std::vector<FoodSource*>::iterator FoodSourceManager::end()
{
	return mFoodSources.end();
}

std::uint32_t FoodSourceManager::foodSourceCount() const
{
	return static_cast<std::uint32_t>(mFoodSources.size());
}

FoodSource& FoodSourceManager::getFoodSource(const std::uint32_t& index)
{
	if (index >= mFoodSources.size())
	{
		throw std::exception("Index out of bounds");
	}

	return *mFoodSources[index];
}
