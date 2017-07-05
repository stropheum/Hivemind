#include "pch.h"
#include "EmployedBee.h"
#include "Hive.h"
#include "FoodSource.h"
#include "FoodSourceManager.h"
#include <cassert>


using namespace std;

EmployedBee::EmployedBee(const sf::Vector2f& position, Hive& hive) :
	Bee(position, hive), mPairedFoodSource(nullptr), mFlowField(position), mDisplayFlowField(false),
	mLineToFoodSource(sf::LineStrip, 2), mFoodSourceData(0.0f, 0.0f), mAbandoningFoodSource(false)
{
	mState = State::Scouting;

	mFillColor = sf::Color::Cyan;
	mBody.setFillColor(mFillColor);
}

void EmployedBee::Update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	UpdateFlowField(window, deltaTime);

	switch (mState)
	{
		case Scouting:
			UpdateScouting(window, deltaTime);
			break;
		case State::SeekingTarget:
			UpdateSeekingTarget(window, deltaTime);
			break;
		case State::HarvestingFood:
			UpdateHarvestingFood(window, deltaTime);
			break;
		case State::DeliveringFood:
			UpdateDeliveringFood(window, deltaTime);
			break;
		case State::DepositingFood:
			UpdateDepositingFood(window, deltaTime);
			break;
		default:
			break;
	}

	if (mPairedFoodSource != nullptr)
	{
		mLineToFoodSource[0].position = mPosition;
		mLineToFoodSource[0].color = sf::Color::Red;
		mLineToFoodSource[1].position = mPairedFoodSource->GetCenterTarget();
		mLineToFoodSource[1].color = sf::Color::Red;
	}

	stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition - sf::Vector2f(mText.getLocalBounds().width / 2.0f, 35));
}

void EmployedBee::Render(sf::RenderWindow& window) const
{
	Bee::Render(window);
	if (mState == State::Scouting && mDisplayFlowField)
	{
		mFlowField.Render(window);
	}

	if (mPairedFoodSource != nullptr && 
		(mState == State::DeliveringFood || mState == State::DepositingFood || mState == State::SeekingTarget))
	{
		window.draw(mLineToFoodSource);
	}
}

void EmployedBee::ToggleFlowField()
{
	mDisplayFlowField = !mDisplayFlowField;
}

void EmployedBee::SetFlowFieldOctaveCount(const std::uint32_t& octaveCount)
{
	mFlowField.SetOctaveCount(octaveCount);
	mFlowField.GenerateNewField();
}

void EmployedBee::WaggleDance()
{
	mParentHive.UpdateKnownFoodSource(mPairedFoodSource, mFoodSourceData);
	mParentHive.TriggerWaggleDance();
}

void EmployedBee::UpdateScouting(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();

	// Onlookers do not scout. Should never meet this condition
	auto rotationRadians = mFlowField.RadianValueAtPosition(mPosition);
	sf::Vector2f newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime
	);

	auto foodSourceManager = FoodSourceManager::GetInstance();
	for (auto iter = foodSourceManager->Begin(); iter != foodSourceManager->End(); ++iter)
	{
		if (CollidingWithFoodSource(*(*iter)))
		{
			mPairedFoodSource = (*iter);
			mTargetFoodSource = (*iter);
			mTargetFoodSource->SetPairedWithEmployee(true);
			SetTarget(mTargetFoodSource->GetCenterTarget());
			mHarvestingClock.restart();
			mState = State::HarvestingFood;
			break;
		}
	}

	UpdatePosition(newPosition, rotationRadians);
}

void EmployedBee::UpdateSeekingTarget(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	auto newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

	UpdatePosition(newPosition, rotationRadians);

	if (DistanceBetween(newPosition, mTarget) <= TARGET_RADIUS)
	{
		mState = State::HarvestingFood;
		mHarvestingClock.restart();
	}
}

void EmployedBee::UpdateHarvestingFood(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	auto newPosition = mPosition;

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
	{	
		mFoodAmount += mTargetFoodSource->TakeFood(EXTRACTION_YIELD);
		if (mTargetFoodSource->GetFoodAmount() == 0.0f)
		{	// We just learned that the food source is no longer viable
			mAbandoningFoodSource = true;
		}
		mFoodSourceData.first = mTargetFoodSource->GetFoodAmount();
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

	UpdatePosition(newPosition, rotationRadians);
}

void EmployedBee::UpdateDeliveringFood(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	mTarget = mParentHive.GetCenterTarget();

	auto newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

	if (DistanceBetween(newPosition, mParentHive.GetCenterTarget()) <= TARGET_RADIUS)
	{
		mState = State::DepositingFood;
		mHarvestingClock.restart();
	}

	UpdatePosition(newPosition, rotationRadians);
}

void EmployedBee::UpdateDepositingFood(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);

	if (DistanceBetween(mTarget, mPosition) <= TARGET_RADIUS)
	{
		auto dimensions = mParentHive.GetDimensions();
		uniform_int_distribution<int> distributionX(static_cast<int>(-dimensions.x / 2), static_cast<int>(dimensions.x / 2));
		uniform_int_distribution<int> distributionY(static_cast<int>(-dimensions.y / 2), static_cast<int>(dimensions.y / 2));
		sf::Vector2f offset(static_cast<float>(distributionX(mGenerator)), static_cast<float>(distributionY(mGenerator)));
		SetTarget(mParentHive.GetCenterTarget() + offset);
	}

	auto newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

	mPosition = newPosition;
	UpdatePosition(newPosition, rotationRadians);

	if (mHarvestingClock.getElapsedTime().asSeconds() >= mHarvestingDuration)
	{	// Now we go back to looking for another food source
		DepositFood(mFoodAmount);
		mTargeting = false;
		if (mPairedFoodSource != nullptr)
		{
			SetTarget(mPairedFoodSource->GetCenterTarget());
		}
		mState = (mPairedFoodSource == nullptr) ? State::Scouting : State::SeekingTarget;
		SetColor(Bee::NORMAL_COLOR);
		WaggleDance();

		if (mAbandoningFoodSource)
		{	// If food source is marked for abandon, we forget about it and tell the hive to forget about it
			mParentHive.RemoveFoodSource(mPairedFoodSource);
			mPairedFoodSource = nullptr;
			mAbandoningFoodSource = false;
		}
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
	if (CollidingWithHive(mParentHive))
	{
		SetColor(Bee::ALERT_COLOR);
	}
}

void EmployedBee::UpdatePosition(const sf::Vector2f& position, const float& rotation)
{
	DetectStructureCollisions();
	mPosition = position;
	mBody.setPosition(sf::Vector2f(mPosition.x - BODY_RADIUS, mPosition.y - BODY_RADIUS));
	mFace.setPosition(mPosition.x, mPosition.y);
	mFace.setRotation(rotation);
}

void EmployedBee::UpdateFlowField(sf::RenderWindow& window, const float& deltaTime)
{
	auto fieldDimensions = mFlowField.GetDimensions();
	if (!mFlowField.CollidingWith(mPosition))
	{	// If we're not colliding with the flow field anymore, reset it on top of us
		mFlowField.SetPosition(sf::Vector2f(mPosition.x - (fieldDimensions.x / 2.0f), mPosition.y - (fieldDimensions.y / 2.0f)));
		mFlowField.GenerateNewField();
	}
	mFlowField.Update(window, deltaTime);
}
