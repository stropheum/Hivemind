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

	updateFlowField(window, deltaTime);

	switch (mState)
	{
		case Scouting:
			updateScouting(window, deltaTime);
			break;
		case State::SeekingTarget:
			updateSeekingTarget(window, deltaTime);
			break;
		case State::HarvestingFood:
			updateHarvestingFood(window, deltaTime);
			break;
		case State::DeliveringFood:
			updateDeliveringFood(window, deltaTime);
			break;
		case State::DepositingFood:
			updateDepositingFood(window, deltaTime);
			break;
		default:
			break;
	}

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

void EmployedBee::updateScouting(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();

	// Onlookers do not scout. Should never meet this condition
	auto rotationRadians = mFlowField.radianValueAtPosition(mPosition);
	sf::Vector2f newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime
	);

	auto foodSourceManager = FoodSourceManager::getInstance();
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

	updatePosition(newPosition, rotationRadians);
}

void EmployedBee::updateSeekingTarget(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	auto newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

	updatePosition(newPosition, rotationRadians);
}

void EmployedBee::updateHarvestingFood(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	auto newPosition = mPosition;

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

	updatePosition(newPosition, rotationRadians);
}

void EmployedBee::updateDeliveringFood(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	mTarget = mParentHive.getCenterTarget();

	auto newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

	if (distanceBetween(newPosition, mParentHive.getCenterTarget()) <= TARGET_RADIUS)
	{
		mState = State::DepositingFood;
		mHarvestingClock.restart();
	}

	updatePosition(newPosition, rotationRadians);
}

void EmployedBee::updateDepositingFood(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);

	if (distanceBetween(mTarget, mPosition) <= TARGET_RADIUS)
	{
		auto dimensions = mParentHive.getDimensions();
		uniform_int_distribution<int> distributionX(static_cast<int>(-dimensions.x / 2), static_cast<int>(dimensions.x / 2));
		uniform_int_distribution<int> distributionY(static_cast<int>(-dimensions.y / 2), static_cast<int>(dimensions.y / 2));
		sf::Vector2f offset(static_cast<float>(distributionX(mGenerator)), static_cast<float>(distributionY(mGenerator)));
		setTarget(mParentHive.getCenterTarget() + offset);
	}

	auto newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

	mPosition = newPosition;
	updatePosition(newPosition, rotationRadians);

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
	if (collidingWithHive(mParentHive))
	{
		setColor(Bee::ALERT_COLOR);
	}
}

void EmployedBee::updatePosition(const sf::Vector2f& position, const float& rotation)
{
	detectStructureCollisions();
	mPosition = position;
	mBody.setPosition(sf::Vector2f(mPosition.x - BODY_RADIUS, mPosition.y - BODY_RADIUS));
	mFace.setPosition(mPosition.x, mPosition.y);
	mFace.setRotation(rotation);
}

void EmployedBee::updateFlowField(sf::RenderWindow& window, const float& deltaTime)
{
	auto fieldDimensions = mFlowField.getDimensions();
	if (!mFlowField.collidingWith(mPosition))
	{	// If we're not colliding with the flow field anymore, reset it on top of us
		mFlowField.setPosition(sf::Vector2f(mPosition.x - (fieldDimensions.x / 2.0f), mPosition.y - (fieldDimensions.y / 2.0f)));
		mFlowField.generateNewField();
	}
	mFlowField.update(window, deltaTime);
}
