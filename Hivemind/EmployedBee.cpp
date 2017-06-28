#include "pch.h"
#include "EmployedBee.h"
#include "Hive.h"
#include "FoodSource.h"
#include "FoodSourceManager.h"
#include <cassert>


using namespace std;

EmployedBee::EmployedBee(const sf::Vector2f& position, Hive& hive): 
	Bee(position, hive), mPairedFoodSource(nullptr), mFlowField(position)
{
	mState = State::SeekingTarget;

	mFillColor = sf::Color::Cyan;
	mBody.setFillColor(mFillColor);
}

void EmployedBee::update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	auto fieldDimensions = mFlowField.getDimensions();
	if (!mFlowField.collidingWith(mPosition))
	{	// If we're not colliding with the flow field anymore, reset it on top of us
		mFlowField.setPosition(sf::Vector2f(mPosition.x - fieldDimensions.x / 2.0f, mPosition.y - fieldDimensions.y / 2.0f));
	}
	mFlowField.update(window, deltaTime);

	auto facePosition = mFace.getPosition();

	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);

	if (mTargetFoodSource != nullptr && mTargetFoodSource->getFoodAmount() == 0)
	{
		mPairedFoodSource = mTargetFoodSource; // TODO: Remove once paired food source is randomly scouted instead of assigned
		mTargeting = false;
		mState = State::SeekingTarget;
		handleFoodSourceCollisions();
	}

	sf::Vector2f newPosition;
	switch (mState)
	{
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
		mTarget = mParentHive.getCenterTarget();
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
		{	// Now we go back to looking for another food source
			depositFood(mFoodAmount);
			mTargeting = false;
			mState = State::SeekingTarget;
			setColor(Bee::NORMAL_COLOR);
			waggleDance();
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

void EmployedBee::render(sf::RenderWindow& window) const
{
	Bee::render(window);
	mFlowField.render(window);
}

void EmployedBee::waggleDance()
{
	// TODO: Deposit information into hive

	// TODO: Iterate over idle bees and dance for them
	if (mTargetFoodSource != nullptr)
	{
		for (auto iter = mParentHive.idleBeesBegin(); iter != mParentHive.idleBeesEnd(); ++iter)
		{
			assert(*iter != nullptr);

			uniform_int_distribution<int> distribution(0, 100);
			int roll = distribution(mGenerator);
			if (roll < 30)
			{	// For now, just a flat 30% roll to simulate waggle selection
				(*iter)->setTarget(mTargetFoodSource);
				(*iter)->setState(State::SeekingTarget);
			}
		}

		mParentHive.validateIdleBees();
	}
}
