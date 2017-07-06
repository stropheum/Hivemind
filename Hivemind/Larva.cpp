#include "pch.h"
#include "Larva.h"


Larva::Larva(const sf::Vector2f& position, Hive& hive, const LarvaType& larvaType) :
	Bee(position, hive),
	mTimeSinceBirth(0.0f), mLarvaDuration(3.0f), mLarvaType(larvaType), mMarkedForDelete(false)
{
	mFillColor = sf::Color::Blue;
	mFace.setSize(sf::Vector2f(0, 0));
	mBody.setRadius(BODY_RADIUS / 3.0f);
}

void Larva::Update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	mTimeSinceBirth += deltaTime;
	if (mTimeSinceBirth >= mLarvaDuration)
	{
		Hatch();
	}
}

void Larva::Render(sf::RenderWindow& window) const
{
	Bee::Render(window);
}

bool Larva::MarkedForDelete() const
{
	return mMarkedForDelete;
}

void Larva::Hatch()
{
	mMarkedForDelete = true;
	auto beeManager = BeeManager::GetInstance();
	switch (mLarvaType)
	{
	case Drone: 
		beeManager->SpawnDrone(mPosition, mParentHive);
		break;
	case Employee:
		beeManager->SpawnEmployee(mPosition, mParentHive);
		break;
	case Onlooker: 
		beeManager->SpawnOnlooker(mPosition, mParentHive);
		break;
	case Queen: 
		beeManager->SpawnQueen(mPosition, mParentHive);
		break;
	case Guard: 
		beeManager->SpawnGuard(mPosition, mParentHive);
		break;
	default: 
		break;
	}
}
