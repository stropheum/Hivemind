#include "pch.h"
#include "Bee.h"
#include "BeeManager.h"
#include "FoodSource.h"
#include "FoodSourceManager.h"
#include "Hive.h"
#include "HiveManager.h"

using namespace std;

const float Bee::STANDARD_BEE_SPEED = 200.0f;
const float Bee::BODY_RADIUS = 12.0f;
const float Bee::TARGET_RADIUS = 5.0f;
const float Bee::STANDARD_HARVESTING_DURATION = 8.0f;
const sf::Color Bee::NORMAL_COLOR = sf::Color(192, 192, 192);
const sf::Color Bee::ALERT_COLOR = sf::Color::Red;
const sf::Color Bee::STANDARD_BODY_COLOR = sf::Color(255, 204, 0);

Bee::Bee(const sf::Vector2f& position, Hive& hive):
	Entity(position, NORMAL_COLOR, STANDARD_BODY_COLOR), mParentHive(hive), mGenerator(), mBody(BODY_RADIUS), 
	mFace(sf::Vector2f(BODY_RADIUS, 2)), mTarget(position), mHarvestingClock(), mSpeed(STANDARD_BEE_SPEED), mFoodAmount(0.0f), 
	mHarvestingDuration(STANDARD_HARVESTING_DURATION), mTargeting(false), mState(State::SeekingTarget), mTargetFoodSource(nullptr)
{
//	mGenerator.seed(static_cast<long>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	std::random_device device;
	mGenerator = std::default_random_engine(device());

	// Randomly offset the bee's speed by a random value
	uniform_real_distribution<float> distribution(-50.0f, 50.0f);
	mSpeed += distribution(mGenerator);

	mBody.setFillColor(mFillColor);
	mBody.setOutlineColor(mOutlineColor);
	mBody.setOutlineThickness(3);
	mFace.setFillColor(sf::Color::White);
	mBody.setPosition(sf::Vector2f(mPosition.x - BODY_RADIUS, mPosition.y - BODY_RADIUS));
	mFace.setPosition(mBody.getPosition().x, mBody.getPosition().y + BODY_RADIUS);

	if (!mFont.loadFromFile("Hack-Regular.ttf"))
	{
		throw exception("Error loading font from file");
	}

	mText.setFont(mFont);
	mText.setCharacterSize(16);
	mText.setOutlineColor(sf::Color::White);
	mText.setFillColor(sf::Color::White);
	stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition - sf::Vector2f(mText.getLocalBounds().width/2.0f, 35));
}

void Bee::render(sf::RenderWindow& window) const
{
	window.draw(mBody);
	if (mState != State::Scouting)
	{	
		window.draw(mFace);
	}
	window.draw(mText);
}

bool Bee::hasTarget() const
{
	return mTargeting;
}

bool Bee::collidingWithFoodSource(const FoodSource& foodSource) const
{
	auto foodPosition = foodSource.getPosition();
	auto foodDimensions = foodSource.getDimensions();
	auto leftWall = foodPosition.x;
	auto rightWall = foodPosition.x + foodDimensions.x;
	auto topWall = foodPosition.y;
	auto bottomWall = foodPosition.y + foodDimensions.y;
	return
		(mPosition.x + BODY_RADIUS > leftWall) &&
		(mPosition.x - BODY_RADIUS < rightWall) &&
		(mPosition.y + BODY_RADIUS > topWall) &&
		(mPosition.y - BODY_RADIUS < bottomWall);
}

bool Bee::collidingWithHive(const Hive& hive) const
{
	auto foodPosition = hive.getPosition();
	auto foodDimensions = hive.getDimensions();
	auto leftWall = foodPosition.x;
	auto rightWall = foodPosition.x + foodDimensions.x;
	auto topWall = foodPosition.y;
	auto bottomWall = foodPosition.y + foodDimensions.y;
	return
		(mPosition.x + BODY_RADIUS > leftWall) &&
		(mPosition.x - BODY_RADIUS < rightWall) &&
		(mPosition.y + BODY_RADIUS > topWall) &&
		(mPosition.y - BODY_RADIUS < bottomWall);
}

void Bee::setColor(const sf::Color& color)
{
	mBody.setOutlineColor(color);
}

void Bee::setTarget(FoodSource* const foodSource)
{
	mTargetFoodSource = foodSource;
	setTarget(foodSource->getCenterTarget());
}

void Bee::setTarget(const sf::Vector2f& position)
{
	mTarget = position;
	mTargeting = true;
}

const sf::Vector2f& Bee::getTarget() const
{
	return mTarget;
}

void Bee::handleFoodSourceCollisions()
{
	auto foodSourceManager = FoodSourceManager::getInstance();
	bool reachedCenterOfSource = false;
	
	if (!hasTarget() && mState == State::SeekingTarget)
	{	// Set initial target
		std::uniform_int_distribution<int> distribution(0, foodSourceManager->getFoodsourceCount() - 1);
		int targetIndex = distribution(mGenerator);
		mTargetFoodSource = &foodSourceManager->getFoodSource(targetIndex);
		sf::Vector2f newTarget = mTargetFoodSource->getCenterTarget();

		setTarget(newTarget);
	}
	switch (mState)
	{
	case State::SeekingTarget:
		for (auto foodIter = foodSourceManager->begin(); foodIter != foodSourceManager->end(); ++foodIter)
		{
			if (collidingWithFoodSource(*(*foodIter)))
			{
				if (Entity::distanceBetween(getPosition(), getTarget()) <= Bee::TARGET_RADIUS)
				{
					reachedCenterOfSource = true;
					mState = State::HarvestingFood;
				}
			}
		}
		break;
	case HarvestingFood: 
		break;
	case DeliveringFood:
		break;
	default: ;
	}

	if (reachedCenterOfSource)
	{
		mState = State::HarvestingFood;
		mHarvestingClock.restart();
	}
}

void Bee::detectStructureCollisions()
{
	auto hiveManager = HiveManager::getInstance();
	auto foodManager = FoodSourceManager::getInstance();

	bool colliding = false;
	for (auto iter = hiveManager->begin(); iter != hiveManager->end(); ++iter)
	{
		if (collidingWithHive(*(*iter)))
		{
			colliding = true;
			break;
		}
	}

	for (auto iter = foodManager->begin(); iter != foodManager->end(); ++iter)
	{
		if (collidingWithFoodSource(*(*iter)))
		{
			colliding = true;
			break;
		}
	}

	setColor(colliding ? Bee::ALERT_COLOR : Bee::NORMAL_COLOR);
}

void Bee::depositFood(float foodAmount)
{
	if (foodAmount > mFoodAmount)
	{	// Cap the food deposit to whatever the maximum is that the bee currently holds
		foodAmount = mFoodAmount;
	}
	mParentHive.depositFood(foodAmount);
	mFoodAmount = 0;
}

void Bee::setState(const State& state)
{
	mState = state;
}

Bee::State Bee::getState() const
{
	return mState;
}
