#include "pch.h"
#include "Bee.h"
#include "BeeManager.h"
#include "FoodSource.h"
#include "FoodSourceManager.h"
#include "Hive.h"

/**
*	@Author: Dale Diaz
*	@Date: 7/21/2017
*/

using namespace std;

const float Bee::STANDARD_BEE_SPEED = 300.0f;
const float Bee::BodyRadius = 12.0f;
const float Bee::TARGET_RADIUS = 5.0f;
const float Bee::STANDARD_HARVESTING_DURATION = 5.0f;
const sf::Color Bee::NORMAL_COLOR = sf::Color(192, 192, 192);
const sf::Color Bee::ALERT_COLOR = sf::Color::Red;
const sf::Color Bee::STANDARD_BODY_COLOR = sf::Color(255, 204, 0);

Bee::Bee(const sf::Vector2f& position, Hive& hive):
	Entity(position, NORMAL_COLOR, STANDARD_BODY_COLOR), mParentHive(hive), mGenerator(), mBody(BodyRadius),
	mFace(sf::Vector2f(BodyRadius, 2)), mTarget(position), mHarvestingClock(), mSpeed(STANDARD_BEE_SPEED), mFoodAmount(0.0f),
	mHarvestingDuration(STANDARD_HARVESTING_DURATION), mMaxEnergy(10.0f), mEnergy(mMaxEnergy), mEnergyConsumptionRate(0.2f), 
	mTargeting(false), mState(State::SeekingTarget), mTargetFoodSource(nullptr)
{
	std::random_device device;
	mGenerator = std::default_random_engine(device());

	// Randomly offset the bee's speed by a random value
	uniform_real_distribution<float> distribution(-50.0f, 50.0f);
	mSpeed += distribution(mGenerator);
	mHarvestingDuration *= (1 / (mSpeed / STANDARD_BEE_SPEED));

	mBody.setPointCount(360);
	mBody.setFillColor(mFillColor);
	mBody.setOutlineColor(mOutlineColor);
	mBody.setOutlineThickness(-2);
	mFace.setFillColor(sf::Color::White);
	mBody.setPosition(sf::Vector2f(mPosition.x - BodyRadius, mPosition.y - BodyRadius));
	mFace.setPosition(mBody.getPosition().x, mBody.getPosition().y + BodyRadius);

	mText.setFont(FontManager::GetInstance()->Hack());
	mText.setCharacterSize(16);
	mText.setOutlineColor(sf::Color::White);
	mText.setFillColor(sf::Color::White);
	stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition - sf::Vector2f(mText.getLocalBounds().width / 2.0f, 35));
}

Bee::~Bee()
{
}

void Bee::Update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	mEnergy -= (mEnergyConsumptionRate * deltaTime);

	if (mState == State::Idle || mState == State::DepositingFood)
	{	
		mEnergy += mParentHive.TakeFood(mMaxEnergy - mEnergy);
	}
	else if (mState == State::HarvestingFood)
	{
		mEnergy += mTargetFoodSource->TakeFood(mMaxEnergy - mEnergy);
	}
	else
	{
		if (Hungry())
		{
			float requiredEnergy = mMaxEnergy - mEnergy;
			if (mFoodAmount < requiredEnergy)
			{
				requiredEnergy = mFoodAmount;
			}
			mEnergy += requiredEnergy;
			mFoodAmount -= requiredEnergy;
		}
	}

	if (mEnergy <= 0.0f)
	{
		MarkForDelete();
	}

	if (mCollisionNode != nullptr && !mCollisionNode->ContainsPoint(mPosition))
	{	// If we haev a collision node and we leave it, invalidate the pointer
		mCollisionNode->UnregisterBee(this);
		mCollisionNode = nullptr;
	}

	if (mCollisionNode == nullptr)
	{	// If the collision node is invalidated, get a new one and register to it
		mCollisionNode = CollisionGrid::GetInstance()->CollisionNodeFromPosition(mPosition);
		mCollisionNode->RegisterBee(this);
	}
}

void Bee::Render(sf::RenderWindow& window) const
{
	window.draw(mBody);
	if (mState != State::Scouting)
	{
		window.draw(mFace);
	}
}

bool Bee::HasTarget() const
{
	return mTargeting;
}

float Bee::GetFoodAmount() const
{
	return mFoodAmount;
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
		(mPosition.x + BodyRadius > leftWall) &&
		(mPosition.x - BodyRadius < rightWall) &&
		(mPosition.y + BodyRadius > topWall) &&
		(mPosition.y - BodyRadius < bottomWall);
}

bool Bee::DetectingFoodSource(const FoodSource& foodSource) const
{
	auto distance = DistanceBetween(mPosition, foodSource.GetCenterTarget()) - Bee::BodyRadius;
	return distance < FoodSource::DetectionRadius;
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
		(mPosition.x + BodyRadius > leftWall) &&
		(mPosition.x - BodyRadius < rightWall) &&
		(mPosition.y + BodyRadius > topWall) &&
		(mPosition.y - BodyRadius < bottomWall);
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
	bool foodSourceFound = false;

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

	auto foodSources = mCollisionNode->FoodSources();
	switch (mState)
	{
	case State::SeekingTarget:
		foodSourceFound = false;
		for (auto iter = foodSources.begin(); iter != foodSources.end(); ++iter)
		{
			if (CollidingWithFoodSource(*(*iter)))
			{
				foodSourceFound = true;
				if (Entity::DistanceBetween(GetPosition(), GetTarget()) <= Bee::TARGET_RADIUS)
				{
					reachedCenterOfSource = true;
					mState = State::HarvestingFood;
					break;
				}
			}
		}

		if (!foodSourceFound)
		{
			auto neighbors = CollisionGrid::GetInstance()->NeighborsOf(mCollisionNode);
			for (uint32_t i = 0; i < neighbors.size(); i++)
			{
				vector<FoodSource*> neighborFoodSources = neighbors[i]->FoodSources();
				for (auto iter = neighborFoodSources.begin(); iter != neighborFoodSources.end(); ++iter)
				{
					if (CollidingWithFoodSource(*(*iter)))
					{
						foodSourceFound = true;
						if (Entity::DistanceBetween(GetPosition(), GetTarget()) <= Bee::TARGET_RADIUS)
						{
							reachedCenterOfSource = true;
							mState = State::HarvestingFood;
							break;
						}
					}
				}
				if (foodSourceFound)
				{	// Early out so we know the others don't matter
					break;
				}
			}
		}
		break;
	default:;
	}

	if (reachedCenterOfSource)
	{
		mState = State::HarvestingFood;
		mHarvestingClock.restart();
	}
}

void Bee::DetectStructureCollisions()
{

	bool colliding = false;
	auto hives = mCollisionNode->Hives();
	for (auto iter = hives.begin(); iter != hives.end(); ++iter)
	{
		if (CollidingWithHive(*(*iter)))
		{
			colliding = true;
			break;
		}
	}

	auto foodSources = mCollisionNode->FoodSources();
	for (auto iter = foodSources.begin(); iter != foodSources.end(); ++iter)
	{
		if (CollidingWithFoodSource(*(*iter)))
		{
			colliding = true;
			break;
		}
	}
	if (!colliding)
	{
		auto neighbors = CollisionGrid::GetInstance()->NeighborsOf(mCollisionNode);
		for (auto iter = neighbors.begin(); iter != neighbors.end(); ++iter)
		{
			auto neighborFoodSources = (*iter)->FoodSources();
			for (auto it = neighborFoodSources.begin(); it != neighborFoodSources.end(); ++it)
			{
				if (CollidingWithFoodSource(*(*it)))
				{
					colliding = true;
					break;
				}
			}
		}
	}

	SetColor(colliding ? Bee::ALERT_COLOR : Bee::NORMAL_COLOR);
}

void Bee::HarvestFood(const float& foodAmount)
{
	mFoodAmount += foodAmount;
}

void Bee::DepositFood(float foodAmount)
{
	if (foodAmount > mFoodAmount)
	{	// Cap the food deposit to whatever the maximum is that the bee currently holds
		foodAmount = mFoodAmount;
	}
	mParentHive.DepositFood(foodAmount);
	mFoodAmount -= foodAmount;
}

void Bee::SetState(const State& state)
{
	mState = state;
}

Bee::State Bee::GetState() const
{
	return mState;
}

bool Bee::Hungry() const
{
	// Hungry if less than 50% energy
	return mEnergy / mMaxEnergy < 0.50f;
}

Hive& Bee::GetParentHive() const
{
	return mParentHive;
}
