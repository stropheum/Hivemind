#include "pch.h"
#include "OnlookerBee.h"
#include "FoodSourceManager.h"
#include "Hive.h"


using namespace std;

OnlookerBee::OnlookerBee(const sf::Vector2f& position, Hive& hive) :
	Bee(position, hive)
{
	mState = State::DeliveringFood;
}

void OnlookerBee::Update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);
	sf::Vector2f newPosition;

	switch (mState)
	{
	case Idle:
		// Wander indefinitely until state is changed
		newPosition = mPosition;

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

		mPosition = newPosition;
		break;
	case Scouting:
		// Onlookers do not scout. Should never meet this condition
		break;

	case State::SeekingTarget:
		newPosition = sf::Vector2f(
			mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
			mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);
		HandleFoodSourceCollisions();
		break;

	case State::HarvestingFood:
		newPosition = mPosition;

		if (mTargetFoodSource != nullptr)
		{
			if (DistanceBetween(mTarget, mPosition) <= TARGET_RADIUS)
			{
				auto dimensions = mTargetFoodSource->GetDimensions();
				uniform_int_distribution<int> distributionX(static_cast<int>(-dimensions.x / 2), static_cast<int>(dimensions.x / 2));
				uniform_int_distribution<int> distributionY(static_cast<int>(-dimensions.y / 2), static_cast<int>(dimensions.y / 2));
				sf::Vector2f offset(static_cast<float>(distributionX(mGenerator)), static_cast<float>(distributionY(mGenerator)));
				SetTarget(mTargetFoodSource->GetCenterTarget() + offset);
			}

			newPosition = sf::Vector2f(
				mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
				mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);
		}

		mPosition = newPosition;

		if (mHarvestingClock.getElapsedTime().asSeconds() >= mHarvestingDuration)
		{	// Now we go back to finding a target
			mFoodAmount += mTargetFoodSource->TakeFood(EXTRACTION_YIELD);
			mTargeting = false;
			mState = State::DeliveringFood;
		}

		SetColor(Bee::NORMAL_COLOR);
		for (auto iter = FoodSourceManager::GetInstance()->Begin(); iter != FoodSourceManager::GetInstance()->End(); ++iter)
		{
			if (CollidingWithFoodSource(*(*iter)))
			{
				SetColor(Bee::ALERT_COLOR);
				break;
			}
		}
		break;

	case State::DeliveringFood:
		SetTarget(mParentHive.GetCenterTarget());
		newPosition = sf::Vector2f(
			mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
			mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);
		if (DistanceBetween(newPosition, mParentHive.GetCenterTarget()) <= TARGET_RADIUS)
		{
			mState = State::DepositingFood;
		}
		break;

	case State::DepositingFood:
		newPosition = mPosition;

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

		mPosition = newPosition;

		if (mHarvestingClock.getElapsedTime().asSeconds() >= mHarvestingDuration)
		{	// Now we go back to finding a target
			DepositFood(mFoodAmount);
			mTargeting = false;
			mState = State::Idle;
			mParentHive.AddIdleBee(this);
			SetColor(Bee::NORMAL_COLOR);
		}

		SetColor(Bee::NORMAL_COLOR);
		for (auto iter = FoodSourceManager::GetInstance()->Begin(); iter != FoodSourceManager::GetInstance()->End(); ++iter)
		{
			if (CollidingWithFoodSource(*(*iter)))
			{
				SetColor(Bee::ALERT_COLOR);
				break;
			}
		}
		break;

	default:
		break;
	}

	DetectStructureCollisions();
	mPosition = newPosition;
	mBody.setPosition(sf::Vector2f(mPosition.x - BodyRadius, mPosition.y - BodyRadius));
	mFace.setPosition(mPosition.x, mPosition.y);
	mFace.setRotation(rotationAngle);

	stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition - sf::Vector2f(mText.getLocalBounds().width / 2.0f, 35));
}

