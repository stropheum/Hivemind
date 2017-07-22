#include "pch.h"
#include "QueenBee.h"


using namespace std;

QueenBee::QueenBee(const sf::Vector2f& position, Hive& hive) :
	Bee(position, hive),
	mLarvaDepositInterval(5.0f), mTimeSinceLarvaDeposit(0.0f)
{
	mFillColor = sf::Color::Magenta;
	mBody.setFillColor(mFillColor);
}

void QueenBee::Update(sf::RenderWindow& window, const float& deltaTime)
{
	Bee::Update(window, deltaTime);

	mTimeSinceLarvaDeposit += deltaTime;
	if (mTimeSinceLarvaDeposit >= mLarvaDepositInterval)
	{
		uniform_int_distribution<int> distribution(0, 3);
		auto roll = distribution(mGenerator);
		switch (roll)
		{
		case 0:
			BeeManager::GetInstance()->SpawnLarva(mPosition, mParentHive, Larva::LarvaType::Onlooker);
			break;
		case 1:
			BeeManager::GetInstance()->SpawnLarva(mPosition, mParentHive, Larva::LarvaType::Employee);
			break;
		case 2:
			BeeManager::GetInstance()->SpawnLarva(mPosition, mParentHive, Larva::LarvaType::Drone);
			break;
		case 3:
			BeeManager::GetInstance()->SpawnLarva(mPosition, mParentHive, Larva::LarvaType::Guard);
			break;
		}
		mTimeSinceLarvaDeposit = 0.0f;
	}

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

void QueenBee::Render(sf::RenderWindow& window) const
{
	Bee::Render(window);
}
