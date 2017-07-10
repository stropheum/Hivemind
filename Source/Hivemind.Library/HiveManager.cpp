#include "pch.h"
#include "HiveManager.h"
#include "Hive.h"

using namespace std;

HiveManager* HiveManager::sInstance = nullptr;

HiveManager::HiveManager()
{
}

HiveManager* HiveManager::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new HiveManager();
	}

	return sInstance;
}

HiveManager::~HiveManager()
{
	for (auto iter = mHives.begin(); iter != mHives.end(); ++iter)
	{
		delete (*iter);
	}
	mHives.clear();
}

Hive& HiveManager::SpawnHive(const sf::Vector2f& position)
{
	mHives.push_back(new Hive(position));
	return (*mHives.back());
}

void HiveManager::Update(sf::RenderWindow& window, const float& deltaTime)
{
	for (auto iter = mHives.begin(); iter != mHives.end(); ++iter)
	{
		(*iter)->Update(window, deltaTime);
	}
}

void HiveManager::Render(sf::RenderWindow& window)
{
	for (auto iter = mHives.begin(); iter != mHives.end(); ++iter)
	{
		(*iter)->Render(window);
	}
}

std::vector<Hive*>::iterator HiveManager::Begin()
{
	return mHives.begin();
}

std::vector<Hive*>::iterator HiveManager::End()
{
	return mHives.end();
}

Hive* HiveManager::GetHive(std::uint32_t index)
{
	if (index >= mHives.size())
	{
		throw exception("Index out of bounds");
	}

	return mHives[index];
}
