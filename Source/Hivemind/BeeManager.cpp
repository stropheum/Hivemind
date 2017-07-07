#include "pch.h"
#include "BeeManager.h"
#include <random>


BeeManager* BeeManager::sInstance = nullptr;

BeeManager::BeeManager():
	mOnlookers(), mEmployees(), mTimeSinceRetarget(0.0f), generator()
{
}

BeeManager* BeeManager::GetInstance()
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
	for (auto iter = mQueens.begin(); iter != mQueens.end(); ++iter)
	{
		delete (*iter);
	}
	for (auto iter = mDrones.begin(); iter != mDrones.end(); ++iter)
	{
		delete (*iter);
	}
	for (auto iter = mGuards.begin(); iter != mGuards.end(); ++iter)
	{
		delete (*iter);
	}
	for (auto iter = mLarva.begin(); iter != mLarva.end(); ++iter)
	{
		delete (*iter);
	}
	mOnlookers.clear();
	mEmployees.clear();
	mQueens.clear();
	mDrones.clear();
	mGuards.clear();
	mLarva.clear();
}

void BeeManager::SpawnOnlooker(const sf::Vector2f& position, Hive& hive)
{
	mOnlookers.push_back(new OnlookerBee(position, hive));
}

void BeeManager::SpawnEmployee(const sf::Vector2f& position, Hive& hive)
{
	mEmployees.push_back(new EmployedBee(position, hive));
}

void BeeManager::SpawnQueen(const sf::Vector2f& position, Hive& hive)
{
	mQueens.push_back(new QueenBee(position, hive));
}

void BeeManager::SpawnDrone(const sf::Vector2f& position, Hive& hive)
{
	mDrones.push_back(new Drone(position, hive));
}

void BeeManager::SpawnGuard(const sf::Vector2f& position, Hive& hive)
{
	mGuards.push_back(new Guard(position, hive));
}

void BeeManager::SpawnLarva(const sf::Vector2f& position, Hive& hive, const Larva::LarvaType& larvaType)
{
	mLarva.push_back(new Larva(position, hive, larvaType));
}

void BeeManager::Update(sf::RenderWindow& window, const float& deltaTime)
{
	for (auto iter = mOnlookers.begin(); iter != mOnlookers.end(); ++iter)
	{
		(*iter)->Update(window, deltaTime);
	}
	for (auto iter = mEmployees.begin(); iter != mEmployees.end(); ++iter)
	{
		(*iter)->Update(window, deltaTime);
	}
	for (auto iter = mQueens.begin(); iter != mQueens.end(); ++iter)
	{
		(*iter)->Update(window, deltaTime);
	}
	for (auto iter = mDrones.begin(); iter != mDrones.end(); ++iter)
	{
		(*iter)->Update(window, deltaTime);
	}
	for (auto iter = mGuards.begin(); iter != mGuards.end(); ++iter)
	{
		(*iter)->Update(window, deltaTime);
	}

	for (auto iter = mLarva.begin(); iter != mLarva.end(); ++iter)
	{
		(*iter)->Update(window, deltaTime);
	}
	CleanupLarva();
}

void BeeManager::Render(sf::RenderWindow& window)
{
	for (auto iter = mOnlookers.begin(); iter != mOnlookers.end(); ++iter)
	{
		(*iter)->Render(window);
	}
	for (auto iter = mEmployees.begin(); iter != mEmployees.end(); ++iter)
	{
		(*iter)->Render(window);
	}
	for (auto iter = mQueens.begin(); iter != mQueens.end(); ++iter)
	{
		(*iter)->Render(window);
	}
	for (auto iter = mDrones.begin(); iter != mDrones.end(); ++iter)
	{
		(*iter)->Render(window);
	}
	for (auto iter = mGuards.begin(); iter != mGuards.end(); ++iter)
	{
		(*iter)->Render(window);
	}
	for (auto iter = mLarva.begin(); iter != mLarva.end(); ++iter)
	{
		(*iter)->Render(window);
	}
}

std::vector<OnlookerBee*>::iterator BeeManager::OnlookerBegin()
{
	return mOnlookers.begin();
}

std::vector<OnlookerBee*>::iterator BeeManager::OnlookerEnd()
{
	return mOnlookers.end();
}

std::vector<EmployedBee*>::iterator BeeManager::EmployeeBegin()
{
	return mEmployees.begin();
}

std::vector<EmployedBee*>::iterator BeeManager::EmployeeEnd()
{
	return mEmployees.end();
}

std::vector<QueenBee*>::iterator BeeManager::QueenBegin()
{
	return mQueens.begin();
}

std::vector<QueenBee*>::iterator BeeManager::QueenEnd()
{
	return mQueens.end();
}

std::vector<Drone*>::iterator BeeManager::DroneBegin()
{
	return mDrones.begin();
}

std::vector<Drone*>::iterator BeeManager::DroneEnd()
{
	return mDrones.end();
}

std::vector<Guard*>::iterator BeeManager::GuardBegin()
{
	return mGuards.begin();
}

std::vector<Guard*>::iterator BeeManager::GuardEnd()
{
	return mGuards.end();
}

std::vector<Larva*>::iterator BeeManager::LarvaBegin()
{
	return mLarva.begin();
}

std::vector<Larva*>::iterator BeeManager::LarvaEnd()
{
	return mLarva.end();
}

std::uint32_t BeeManager::OnlookerCount() const
{
	return static_cast<std::uint32_t>(mOnlookers.size());
}

std::uint32_t BeeManager::EmployeeCount() const
{
	return static_cast<std::uint32_t>(mEmployees.size());
}

std::uint32_t BeeManager::QueenCount() const
{
	return static_cast<std::uint32_t>(mQueens.size());
}

std::uint32_t BeeManager::DroneCount() const
{
	return static_cast<std::uint32_t>(mDrones.size());
}

std::uint32_t BeeManager::GuardCount() const
{
	return static_cast<std::uint32_t>(mGuards.size());
}

std::uint32_t BeeManager::LarvaCount() const
{
	return static_cast<std::uint32_t>(mLarva.size());
}

void BeeManager::ToggleEmployeeFlowFields()
{
	for (auto iter = mEmployees.begin(); iter != mEmployees.end(); ++iter)
	{
		(*iter)->ToggleFlowField();
	}
}

void BeeManager::SetEmployeeFlowFieldOctaveCount(const std::uint32_t& octaveCount)
{
	for (auto iter = mEmployees.begin(); iter != mEmployees.end(); ++iter)
	{
		(*iter)->SetFlowFieldOctaveCount(octaveCount);
	}
}

void BeeManager::CleanupLarva()
{
	bool larvaRemoved = true;
	while (larvaRemoved)
	{
		larvaRemoved = false;
		for (auto iter = mLarva.begin(); iter != mLarva.end(); ++iter)
		{
			if ((*iter)->MarkedForDelete())
			{
				delete *iter;
				mLarva.erase(iter);
				larvaRemoved = true;
				break;
			}
		}
	}
}
