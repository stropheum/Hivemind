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
const float Bee::STANDARD_HARVESTING_DURATION = 5.0f;
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

void Bee::Render(sf::RenderWindow& window) const
{
	window.draw(mBody);
	if (mState != State::Scouting)
	{	
		window.draw(mFace);
	}
//	window.draw(mText);
}

bool Bee::HasTarget() const
{
	return mTargeting;
}

bool Bee::CollidingWithFoodSource(const FoodSource& foodSource) const
{
	auto foodPosition = foodSource.GetPosition();
	auto foodDimensions = foodSource.GetDimensions();
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

bool Bee::CollidingWithHive(const Hive& hive) const
{
	auto foodPosition = hive.GetPosition();
	auto foodDimensions = hive.GetDimensions();
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

void Bee::SetColor(const sf::Color& color)
{
	mBody.setOutlineColor(color);
}

void Bee::SetTarget(FoodSource* const foodSource)
{
	mTargetFoodSource = foodSource;
	SetTarget(foodSource->GetCenterTarget());
}

void Bee::SetTarget(const sf::Vector2f& position)
{
	mTarget = position;
	mTargeting = true;
}

const sf::Vector2f& Bee::GetTarget() const
{
	return mTarget;
}

void Bee::HandleFoodSourceCollisions()
{
	auto foodSourceManager = FoodSourceManager::GetInstance();
	bool reachedCenterOfSource = false;
	
	if (!HasTarget() && mState == State::SeekingTarget)
	{	// Set initial target
		mTargeting = true;
		std::uniform_int_distribution<int> distribution(0, foodSourceManager->GetFoodSourceCount() - 1);
		int targetIndex = distribution(mGenerator);
		mTargetFoodSource = &foodSourceManager->GetFoodSource(targetIndex);
		sf::Vector2f newTarget = mTargetFoodSource->GetCenterTarget();

		SetTarget(newTarget);
		mState = State::DeliveringFood;
	}
	
	switch (mState)
	{
	case State::SeekingTarget:
		for (auto foodIter = foodSourceManager->Begin(); foodIter != foodSourceManager->End(); ++foodIter)
		{
			if (CollidingWithFoodSource(*(*foodIter)))
			{
				if (Entity::DistanceBetween(GetPosition(), GetTarget()) <= Bee::TARGET_RADIUS)
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

void Bee::DetectStructureCollisions()
{
	auto hiveManager = HiveManager::GetInstance();
	auto foodManager = FoodSourceManager::GetInstance();

	bool colliding = false;
	for (auto iter = hiveManager->Begin(); iter != hiveManager->End(); ++iter)
	{
		if (CollidingWithHive(*(*iter)))
		{
			colliding = true;
			break;
		}
	}

	for (auto iter = foodManager->Begin(); iter != foodManager->End(); ++iter)
	{
		if (CollidingWithFoodSource(*(*iter)))
		{
			colliding = true;
			break;
		}
	}

	SetColor(colliding ? Bee::ALERT_COLOR : Bee::NORMAL_COLOR);
}

void Bee::DepositFood(float foodAmount)
{
	if (foodAmount > mFoodAmount)
	{	// Cap the food deposit to whatever the maximum is that the bee currently holds
		foodAmount = mFoodAmount;
	}
	mParentHive.DepositFood(foodAmount);
	mFoodAmount = 0;
}

void Bee::SetState(const State& state)
{
	mState = state;
}

Bee::State Bee::GetState() const
{
	return mState;
}
