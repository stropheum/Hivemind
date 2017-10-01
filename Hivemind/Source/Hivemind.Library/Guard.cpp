#include "pch.h"
#include "Guard.h"


using namespace std;

Guard::Guard(const sf::Vector2f& position, Hive& hive) :
	Bee(position, hive)
{
	mFillColor = sf::Color(128, 0, 0);
	mBody.setFillColor(mFillColor);
}

void Guard::Update(sf::RenderWindow& window, const double& deltaTime)
{
	Bee::Update(window, deltaTime);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);
	sf::Vector2f newPosition = mPosition;

	if (DistanceBetween(mTarget, mPosition) <= TARGET_RADIUS)
	{
		auto dimensions = mParentHive.GetDimensions();
		uniform_int_distribution<int> distributionX(static_cast<int>(-dimensions.x), static_cast<int>(dimensions.x));
		uniform_int_distribution<int> distributionY(static_cast<int>(-dimensions.y), static_cast<int>(dimensions.y));
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

	auto waspManager = WaspManager::GetInstance();
	for (auto iter = waspManager->Begin(); iter != waspManager->End(); ++iter)
	{
		if ((*iter)->GetTargetHive() == &mParentHive && !(*iter)->MarkedForDelete() && 
			DistanceBetween(mParentHive.GetCenterTarget(), (*iter)->GetPosition()) < mParentHive.GetDimensions().x)
		{
			MarkForDelete();
			(*iter)->MarkForDelete();
			break;
		}
	}
}

void Guard::Render(sf::RenderWindow& window) const
{
	Bee::Render(window);
}
