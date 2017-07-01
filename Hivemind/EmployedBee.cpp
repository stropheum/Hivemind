#include "pch.h"
#include "EmployedBee.h"
#include "Hive.h"
#include "FoodSource.h"
#include "FoodSourceManager.h"
#include <cassert>


using namespace std;

EmployedBee::EmployedBee(const sf::Vector2f& position, Hive& hive): 
	Bee(position, hive), mPairedFoodSource(nullptr), mFlowField(position), mDisplayFlowField(false), 
	mLineToFoodSource(sf::LineStrip, 2), mFoodSourceData(0.0f, 0.0f)
{
	mState = State::Scouting;

	mFillColor = sf::Color::Cyan;
	mBody.setFillColor(mFillColor);
}

void EmployedBee::update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	auto foodSourceManager = FoodSourceManager::getInstance();

	auto fieldDimensions = mFlowField.getDimensions();
	if (!mFlowField.collidingWith(mPosition))
	{	// If we're not colliding with the flow field anymore, reset it on top of us
		mFlowField.setPosition(sf::Vector2f(mPosition.x - (fieldDimensions.x / 2.0f), mPosition.y - (fieldDimensions.y / 2.0f)));
		mFlowField.generateNewField();
	}
	mFlowField.update(window, deltaTime);

	auto facePosition = mFace.getPosition();

	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);

	// Re-enable to have scout ignore food source
	// TODO: have scouts pair with food source, and ignore empty food sources unless already paired
//	if (mTargetFoodSource != nullptr && mTargetFoodSource->getFoodAmount() == 0)
//	{
//		mTargetFoodSource = nullptr;
//		mTargeting = false;
//	}

	sf::Vector2f newPosition;
	switch (mState)
	{
	case Scouting:
		// Onlookers do not scout. Should never meet this condition
		rotationRadians = mFlowField.radianValueAtPosition(mPosition);
		newPosition = sf::Vector2f(
			mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
			mPosition.y + sin(rotationRadians) * mSpeed * deltaTime
		);
		
		for (auto iter = foodSourceManager->begin(); iter != foodSourceManager->end(); ++iter)
		{
			if (collidingWithFoodSource(*(*iter)))
			{
				mPairedFoodSource = (*iter);
				mTargetFoodSource = (*iter);
				mTargetFoodSource->setPairedWithEmployee(true);
				setTarget(mTargetFoodSource->getCenterTarget());
				mHarvestingClock.restart();
				mState = State::HarvestingFood;
				break;
			}
		}
		break;

	case State::SeekingTarget:
		newPosition = sf::Vector2f(
			mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
			mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

//		handleFoodSourceCollisions();
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
			mFoodSourceData.first = mTargetFoodSource->getFoodAmount();
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
			mHarvestingClock.restart();
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
			mState = State::Scouting;
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

	if (mPairedFoodSource != nullptr)
	{
		mLineToFoodSource[0].position = mPosition;
		mLineToFoodSource[0].color = sf::Color::Red;
		mLineToFoodSource[1].position = mPairedFoodSource->getCenterTarget();
		mLineToFoodSource[1].color = sf::Color::Red;
	}

	stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition - sf::Vector2f(mText.getLocalBounds().width / 2.0f, 35));
}

void EmployedBee::render(sf::RenderWindow& window) const
{
	Bee::render(window);
	if (mState == State::Scouting && mDisplayFlowField)
	{
		mFlowField.render(window);
	}

	if (mPairedFoodSource != nullptr)
	{
		window.draw(mLineToFoodSource);
	}
}

void EmployedBee::toggleFlowField()
{
	mDisplayFlowField = !mDisplayFlowField;
}

void EmployedBee::setFlowFieldOctaveCount(const std::uint32_t& octaveCount)
{
	mFlowField.setOctaveCount(octaveCount);
	mFlowField.generateNewField();
}

void EmployedBee::waggleDance()
{
	mParentHive.updateKnownFoodSource(mPairedFoodSource, mFoodSourceData);
	mParentHive.triggerWaggleDance();
}
