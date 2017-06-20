#include "pch.h"
#include "HiveManager.h"
#include "Hive.h"

using namespace std;

HiveManager* HiveManager::sInstance = nullptr;

HiveManager::HiveManager()
{
}

HiveManager* HiveManager::getInstance()
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

void HiveManager::spawnHive(const sf::Vector2f& position)
{
	mHives.push_back(new Hive(position));
}

void HiveManager::update(sf::RenderWindow& window, const float& deltaTime)
{
	for (auto iter = mHives.begin(); iter != mHives.end(); ++iter)
	{
		(*iter)->update(window, deltaTime);
	}
}

void HiveManager::render(sf::RenderWindow& window)
{
	for (auto iter = mHives.begin(); iter != mHives.end(); ++iter)
	{
		(*iter)->render(window);
	}
}

std::vector<Hive*>::iterator HiveManager::begin()
{
	return mHives.begin();
}

std::vector<Hive*>::iterator HiveManager::end()
{
	return mHives.end();
}

Hive* HiveManager::getHive(std::uint32_t index)
{
	if (index >= mHives.size())
	{
		throw exception("Index out of bounds");
	}

	return mHives[index];
}
