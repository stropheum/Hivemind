#include "pch.h"
#include "OnlookerBee.h"
#include "FoodSourceManager.h"
#include "Hive.h"


using namespace std;

OnlookerBee::OnlookerBee(const sf::Vector2f& position, Hive& hive): 
	Bee(position, hive)
{
	mState = State::DeliveringFood;
}

void OnlookerBee::update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();

	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);

//	if (mTargetFoodSource != nullptr && mTargetFoodSource->getFoodAmount() == 0)
//	{
//		mTargeting = false;
//		mState = State::SeekingTarget;
//		handleFoodSourceCollisions();
//	}

	sf::Vector2f newPosition;
	switch (mState)
	{
	case Idle:
		// Wander indefinitely until state is changed
		newPosition = mPosition;

		if (distanceBetween(mTarget, mPosition) <= TARGET_RADIUS)
		{
			auto dimensions = mParentHive.getDimensions();
			uniform_int_distribution<int> distributionX(static_cast<int>(-dimensions.x / 2), static_cast<int>(dimensions.x / 2));
			uniform_int_distribution<int> distributionY(static_cast<int>(-dimensions.y / 2), static_cast<int>(dimensions.y / 2));
			sf::Vector2f offset(static_cast<float>(distributionX(mGenerator)), static_cast<float>(distributionY(mGenerator)));
			setTarget(mParentHive.getCenterTarget() + offset);
		}

		newPosition = sf::Vector2f(
			mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
			mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

		mPosition = newPosition;
		break;
	case Scouting: 
		// Onlookers do not scout. Should never meet this condition
		break;

	case State::SeekingTarget:
		newPosition = sf::Vector2f(
			mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
			mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);
		handleFoodSourceCollisions();
		break;

	case State::HarvestingFood:
		newPosition = mPosition;

		if (mTargetFoodSource != nullptr)
		{
			if (distanceBetween(mTarget, mPosition) <= TARGET_RADIUS)
			{
				auto dimensions = mTargetFoodSource->getDimensions();
				uniform_int_distribution<int> distributionX(static_cast<int>(-dimensions.x / 2), static_cast<int>(dimensions.x / 2));
				uniform_int_distribution<int> distributionY(static_cast<int>(-dimensions.y / 2), static_cast<int>(dimensions.y / 2));
				sf::Vector2f offset(static_cast<float>(distributionX(mGenerator)), static_cast<float>(distributionY(mGenerator)));
				setTarget(mTargetFoodSource->getCenterTarget() + offset);
			}

			newPosition = sf::Vector2f(
				mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
				mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);
		}

		mPosition = newPosition;

		if (mHarvestingClock.getElapsedTime().asSeconds() >= mHarvestingDuration)
		{	// Now we go back to finding a target
			mFoodAmount += mTargetFoodSource->takeFood(EXTRACTION_YIELD);
			mTargeting = false;
			mState = State::DeliveringFood;
		}

		setColor(Bee::NORMAL_COLOR);
		for (auto iter = FoodSourceManager::getInstance()->begin(); iter != FoodSourceManager::getInstance()->end(); ++iter)
		{
			if (collidingWithFoodSource(*(*iter)))
			{
				setColor(Bee::ALERT_COLOR);
				break;
			}
		}
		break;

	case State::DeliveringFood:
		setTarget(mParentHive.getCenterTarget());
		newPosition = sf::Vector2f(
			mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
			mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);
		if (distanceBetween(newPosition, mParentHive.getCenterTarget()) <= TARGET_RADIUS)
		{
			mState = State::DepositingFood;
		}
		break;

	case State::DepositingFood:
		newPosition = mPosition;

		if (distanceBetween(mTarget, mPosition) <= TARGET_RADIUS)
		{
			auto dimensions = mParentHive.getDimensions();
			uniform_int_distribution<int> distributionX(static_cast<int>(-dimensions.x / 2), static_cast<int>(dimensions.x / 2));
			uniform_int_distribution<int> distributionY(static_cast<int>(-dimensions.y / 2), static_cast<int>(dimensions.y / 2));
			sf::Vector2f offset(static_cast<float>(distributionX(mGenerator)), static_cast<float>(distributionY(mGenerator)));
			setTarget(mParentHive.getCenterTarget() + offset);
		}

		newPosition = sf::Vector2f(
			mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
			mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

		mPosition = newPosition;

		if (mHarvestingClock.getElapsedTime().asSeconds() >= mHarvestingDuration)
		{	// Now we go back to finding a target
			depositFood(mFoodAmount);
			mTargeting = false;
			mState = State::Idle;
			mParentHive.addIdleBee(this);
			setColor(Bee::NORMAL_COLOR);
		}

		setColor(Bee::NORMAL_COLOR);
		for (auto iter = FoodSourceManager::getInstance()->begin(); iter != FoodSourceManager::getInstance()->end(); ++iter)
		{
			if (collidingWithFoodSource(*(*iter)))
			{
				setColor(Bee::ALERT_COLOR);
				break;
			}
		}
		break;
	
	default: 
		break;
	}

	detectStructureCollisions();
	mPosition = newPosition;
	mBody.setPosition(sf::Vector2f(mPosition.x - BODY_RADIUS, mPosition.y - BODY_RADIUS));
	mFace.setPosition(mPosition.x, mPosition.y);
	mFace.setRotation(rotationAngle);

	stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition - sf::Vector2f(mText.getLocalBounds().width / 2.0f, 35));
}

