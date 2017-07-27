#include "pch.h"
#include "WaspManager.h"

/**
 * @Author: Dale Diaz
 * @Date: 7/27/2017
 */

using namespace std;

WaspManager* WaspManager::sInstance = nullptr;

WaspManager::WaspManager():
	mTimeSinceSpawn(0.0f)
{
	std::random_device device;
	mGenerator = std::default_random_engine(device());
}

void WaspManager::CleanupWasps()
{
	bool waspRemoved = true;
	while (waspRemoved)
	{
		waspRemoved = false;
		for (auto iter = mWasps.begin(); iter != mWasps.end(); ++iter)
		{
			if ((*iter)->MarkedForDelete())
			{
				delete (*iter);
				mWasps.erase(iter);
				waspRemoved = true;
				break;
			}
		}
	}
}

WaspManager::~WaspManager()
{
	for (auto iter = mWasps.begin(); iter != mWasps.end(); ++iter)
	{
		delete (*iter);
	}
	mWasps.clear();
}

WaspManager* WaspManager::GetInstance()
{
	if (sInstance == nullptr)
	{
		sInstance = new WaspManager();
	}
	return sInstance;
}

std::vector<Wasp*>::iterator WaspManager::Begin()
{
	return mWasps.begin();
}

std::vector<Wasp*>::iterator WaspManager::End()
{
	return mWasps.end();
}

void WaspManager::Update(sf::RenderWindow& window, const double& deltaTime)
{
	mTimeSinceSpawn += deltaTime;
	if (mTimeSinceSpawn >= mSpawnInterval)
	{
		mTimeSinceSpawn = 0.0f;
		uniform_real_distribution<float> distribution(-10000.0f, 10000.0f);
		SpawnWasp(sf::Vector2f(distribution(mGenerator), distribution(mGenerator)));
	}

	for (auto iter = mWasps.begin(); iter != mWasps.end(); ++iter)
	{
		(*iter)->Update(window, deltaTime);
	}
	CleanupWasps();
}

void WaspManager::Render(sf::RenderWindow& window) const
{
	for (auto iter = mWasps.begin(); iter != mWasps.end(); ++iter)
	{
		(*iter)->Render(window);
	}
}

void WaspManager::SpawnWasp(const sf::Vector2f& position)
{
	mWasps.push_back(new Wasp(position));
}
