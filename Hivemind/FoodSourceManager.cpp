#include "pch.h"
#include "FoodSourceManager.h"
#include "FoodSource.h"


FoodSourceManager* FoodSourceManager::sInstance = nullptr;

FoodSourceManager::FoodSourceManager()
{
}

FoodSourceManager* FoodSourceManager::GetInstance()
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

void FoodSourceManager::SpawnFoodSource(const sf::Vector2f& position)
{
	mFoodSources.push_back(new FoodSource(position));
}

void FoodSourceManager::Update(sf::RenderWindow& window, const float& deltaTime)
{
	for (auto iter = mFoodSources.begin(); iter != mFoodSources.end(); ++iter)
	{
		(*iter)->Update(window, deltaTime);
	}
}

void FoodSourceManager::Render(sf::RenderWindow& window)
{
	for (auto iter = mFoodSources.begin(); iter != mFoodSources.end(); ++iter)
	{
		(*iter)->Render(window);
	}
}

std::uint32_t FoodSourceManager::GetFoodsourceCount() const
{
	return static_cast<std::uint32_t>(mFoodSources.size());
}

std::vector<FoodSource*>::iterator FoodSourceManager::Begin()
{
	return mFoodSources.begin();
}

std::vector<FoodSource*>::iterator FoodSourceManager::End()
{
	return mFoodSources.end();
}

std::uint32_t FoodSourceManager::FoodSourceCount() const
{
	return static_cast<std::uint32_t>(mFoodSources.size());
}

FoodSource& FoodSourceManager::GetFoodSource(const std::uint32_t& index)
{
	if (index >= mFoodSources.size())
	{
		throw std::exception("Index out of bounds");
	}

	return *mFoodSources[index];
}
