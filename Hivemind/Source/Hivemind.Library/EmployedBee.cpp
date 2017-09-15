#include "pch.h"
#include "EmployedBee.h"
#include "Hive.h"
#include "FoodSource.h"
#include "FoodSourceManager.h"


using namespace std;

EmployedBee::EmployedBee(const sf::Vector2f& position, Hive& hive) :
	Bee(position, hive), mPairedFoodSource(nullptr), mDisplayFlowField(false),
	mLineToFoodSource(sf::LineStrip, 2), mFoodSourceData(0.0f, 0.0f), mAbandoningFoodSource(false)
{
	mState = State::Scouting;
	mFillColor = sf::Color::Cyan;
	mBody.setFillColor(mFillColor);

	EmployedBee::PopulateFunctionMaps();
}

void EmployedBee::Update(sf::RenderWindow& window, const double& deltaTime)
{
	Bee::Update(window, deltaTime);

	assert(mState != State::Idle);
	mUpdate[mState](window, deltaTime);

	if (mPairedFoodSource != nullptr)
	{
		mLineToFoodSource[0].position = mPosition;
		mLineToFoodSource[0].color = sf::Color(255, 0, 0, 64);
		mLineToFoodSource[1].position = mPairedFoodSource->GetCenterTarget();
		mLineToFoodSource[1].color = sf::Color(255, 0, 0, 64);
	}
}

void EmployedBee::Render(sf::RenderWindow& window) const
{
	Bee::Render(window);
	if (mState == State::Scouting && mDisplayFlowField)
	{
	}

	if (mPairedFoodSource != nullptr && mState != State::Scouting)
	{
		//		window.draw(mLineToFoodSource);
	}
}

void EmployedBee::ToggleFlowField()
{
	mDisplayFlowField = !mDisplayFlowField;
}

void EmployedBee::SetFlowFieldOctaveCount(const std::uint32_t& octaveCount)
{
	UNREFERENCED_PARAMETER(octaveCount);
	//	mFlowField = FlowFieldManager::GetInstance()->GetField();
	//	mFlowField.SetOctaveCount(octaveCount);
}

void EmployedBee::PopulateFunctionMaps()
{
	mUpdate[State::Scouting] = [&](sf::RenderWindow& window, const double& deltaTime)
	{ this->UpdateScouting(window, deltaTime); };

	mUpdate[State::SeekingTarget] = [&](sf::RenderWindow& window, const double& deltaTime)
	{ this->UpdateSeekingTarget(window, deltaTime); };

	mUpdate[State::HarvestingFood] = [&](sf::RenderWindow& window, const double& deltaTime)
	{ this->UpdateHarvestingFood(window, deltaTime); };

	mUpdate[State::DeliveringFood] = [&](sf::RenderWindow& window, const double& deltaTime)
	{ this->UpdateDeliveringFood(window, deltaTime); };

	mUpdate[State::DepositingFood] = [&](sf::RenderWindow& window, const double& deltaTime)
	{ this->UpdateDepositingFood(window, deltaTime); };
}

void EmployedBee::WaggleDance() const
{
	mParentHive.UpdateKnownFoodSource(mPairedFoodSource, mFoodSourceData);
	mParentHive.TriggerWaggleDance();
}

void EmployedBee::UpdateScouting(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto bounds = 10000;
	if (mPosition.x < mParentHive.GetCenterTarget().x - bounds || mPosition.x > mParentHive.GetCenterTarget().x + bounds ||
		mPosition.y < mParentHive.GetCenterTarget().y - bounds || mPosition.y > mParentHive.GetCenterTarget().y + bounds ||
		(mEnergy / mMaxEnergy) < 0.30f)
	{
		mState = State::DeliveringFood;
	}

	sf::Vector2f newPosition = mPosition;
	float rotationRadians = atan2(mTarget.y - mPosition.y, mTarget.x - mPosition.x);
	newPosition.x += (cos(rotationRadians) * mSpeed * deltaTime);
	newPosition.y += (sin(rotationRadians) * mSpeed * deltaTime);

	if (DistanceBetween(mPosition, mTarget) <= mBody.getRadius())
	{
		GenerateNewTarget();
	}

	auto foodSources = FoodSourceManager::GetInstance();
	for (auto iter = foodSources->Begin(); iter != foodSources->End(); ++iter)
	{
		if (DetectingFoodSource(*(*iter)) && !(*iter)->ContainsRegisteredHive(&mParentHive))//!(*iter)->PairedWithEmployee())
		{
			mPairedFoodSource = (*iter);
			mTargetFoodSource = (*iter);
			mTargetFoodSource->SetPairedWithEmployee(true);
			mTargetFoodSource->RegisterHive(&mParentHive);
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
		HarvestFood(mTargetFoodSource->TakeFood(EXTRACTION_YIELD));
		if (mTargetFoodSource->GetFoodAmount() == 0.0f)
		{	// We just learned that the food source is no longer viable
			mAbandoningFoodSource = true;
		}
		mFoodSourceData.first = mTargetFoodSource->GetFoodAmount();
		mFoodSourceData.second = DistanceBetween(mTargetFoodSource->GetCenterTarget(), mParentHive.GetCenterTarget());
		mTargeting = false;
		mState = State::DeliveringFood;
	}

	SetColor(Bee::NORMAL_COLOR);

	auto foodSources = mCollisionNode->FoodSources();
	bool foodSourceFound = false;
	for (auto iter = foodSources.begin(); iter != foodSources.end(); ++iter)
	{
		if (CollidingWithFoodSource(*(*iter)))
		{
			foodSourceFound = true;
			SetColor(Bee::ALERT_COLOR);
			break;
		}
	}

	if (!foodSourceFound)
	{	// We need to search the neighbors now
		vector<CollisionNode*> neighbors = CollisionGrid::GetInstance()->NeighborsOf(mCollisionNode);
		for (uint32_t i = 0; i < neighbors.size(); i++)
		{
			auto neighborFoodSources = neighbors[i]->FoodSources();
			for (auto iter = neighborFoodSources.begin(); iter != neighborFoodSources.end(); ++iter)
			{
				foodSourceFound = true;
				SetColor(Bee::ALERT_COLOR);
				break;
			}
			if (foodSourceFound)
			{	// Early out if we find a collision
				break;
			}
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
			if (mPairedFoodSource != nullptr)
			{
				mPairedFoodSource->SetPairedWithEmployee(false);
				mPairedFoodSource->UnregisterHive(&mParentHive);
			}
			mPairedFoodSource = nullptr;
			mAbandoningFoodSource = false;
		}
	}

	SetColor(Bee::NORMAL_COLOR);

	auto foodSources = mCollisionNode->FoodSources();
	bool foodSourceFound = false;
	for (auto iter = foodSources.begin(); iter != foodSources.end(); ++iter)
	{
		if (CollidingWithFoodSource(*(*iter)))
		{
			foodSourceFound = true;
			SetColor(Bee::ALERT_COLOR);
			break;
		}
	}

	if (!foodSourceFound)
	{
		vector<CollisionNode*> neighbors = CollisionGrid::GetInstance()->NeighborsOf(mCollisionNode);
		for (uint32_t i = 0; i < neighbors.size(); i++)
		{
			auto neighborFoodSources = neighbors[i]->FoodSources();
			for (auto iter = neighborFoodSources.begin(); iter != neighborFoodSources.end(); ++iter)
			{
				if (CollidingWithFoodSource(*(*iter)))
				{
					foodSourceFound = true;
					SetColor(Bee::ALERT_COLOR);
					break;
				}
			}
			if (foodSourceFound)
			{	// Early out if we found a collision
				break;
			}
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
	mBody.setPosition(sf::Vector2f(mPosition.x - BodyRadius, mPosition.y - BodyRadius));
	mFace.setPosition(mPosition.x, mPosition.y);
	auto rotationAngle = rotation * (180.0f / PI);
	mFace.setRotation(rotationAngle);
}

void EmployedBee::GenerateNewTarget()
{
	uniform_real_distribution<float> distribution(-500.0f, 500.0f);
	sf::Vector2f offset(distribution(mGenerator), distribution(mGenerator));
	mTarget = mPosition + offset;
}
