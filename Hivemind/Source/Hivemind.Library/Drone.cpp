#include "pch.h"
#include "Drone.h"


using namespace std;

Drone::Drone(const sf::Vector2f& position, Hive& hive) :
	Bee(position, hive)
{
	mFillColor = sf::Color(128, 128, 128);
	mBody.setFillColor(mFillColor);
}

Drone::~Drone()
{
}

void Drone::Update(sf::RenderWindow& window, const double& deltaTime)
{
	Bee::Update(window, deltaTime);

	HandleCombManagement(deltaTime);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);
	sf::Vector2f newPosition = mPosition;

	if (DistanceBetween(mTarget, mPosition) <= TARGET_RADIUS)
	{
		auto dimensions = mParentHive.GetDimensions();
		uniform_int_distribution<int> distributionX(static_cast<int>(-dimensions.x / 2), static_cast<int>(dimensions.x / 2));
		uniform_int_distribution<int> distributionY(static_cast<int>(-dimensions.y / 2), static_cast<int>(dimensions.y / 2));
		sf::Vector2f offset(static_cast<float>(distributionX(mGenerator)), static_cast<float>(distributionY(mGenerator)));
		SetTarget(mParentHive.GetCenterTarget() + offset);
	}

	newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

	DetectStructureCollisions();
	mPosition = newPosition;
	mBody.setPosition(sf::Vector2f(mPosition.x - BodyRadius, mPosition.y - BodyRadius));
	mFace.setPosition(mPosition.x, mPosition.y);
	mFace.setRotation(rotationAngle);
}

void Drone::Render(sf::RenderWindow& window) const
{
	Bee::Render(window);
}

void Drone::HandleCombManagement(const float& deltaTime) const
{
	if (mParentHive.RequiresHoneyComb())
	{
		mParentHive.ConvertToHoneyComb(mEnergyConsumptionRate * 50 * deltaTime);
	}

	if (mParentHive.RequiresBroodComb())
	{
		mParentHive.ConvertToBroodComb(mEnergyConsumptionRate * deltaTime);
	}

	if (mParentHive.RequiresStructuralComb())
	{
		mParentHive.AddStructuralComb(mEnergyConsumptionRate * 25 * deltaTime);
	}
}
