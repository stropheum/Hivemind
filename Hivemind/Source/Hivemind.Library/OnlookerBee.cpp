#include "pch.h"
#include "OnlookerBee.h"


using namespace std;

OnlookerBee::OnlookerBee(const sf::Vector2f& position, Hive& hive) :
	Bee(position, hive)
{
	mState = State::DeliveringFood;
	OnlookerBee::PopulateFunctionMaps();
}

void OnlookerBee::Update(sf::RenderWindow& window, const double& deltaTime)
{
	Bee::Update(window, deltaTime);

	if (MarkedForDelete())
	{
		mParentHive.RemoveIdleBee(this);
	}

	assert(mState != State::Scouting);
	mUpdate[mState](window, deltaTime);
}

void OnlookerBee::PopulateFunctionMaps()
{
	mUpdate[State::Idle] = [&](sf::RenderWindow& window, const double& deltaTime)
	{ this->UpdateIdle(window, deltaTime); };

	mUpdate[State::SeekingTarget] = [&](sf::RenderWindow& window, const double& deltaTime)
	{ this->UpdateSeekingTarget(window, deltaTime); };

	mUpdate[State::HarvestingFood] = [&](sf::RenderWindow& window, const double& deltaTime)
	{ this->UpdateHarvestingFood(window, deltaTime); };

	mUpdate[State::DeliveringFood] = [&](sf::RenderWindow& window, const double& deltaTime)
	{ this->UpdateDeliveringFood(window, deltaTime); };

	mUpdate[State::DepositingFood] = [&](sf::RenderWindow& window, const double& deltaTime)
	{ this->UpdateDepositingFood(window, deltaTime); };
}

void OnlookerBee::UpdateIdle(sf::RenderWindow& window, const double& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);

	if (DistanceBetween(mTarget, mPosition) <= TARGET_RADIUS)
	{
		auto dimensions = mParentHive.GetDimensions();
		uniform_int_distribution<int> distributionX(static_cast<int>(-dimensions.x / 2), static_cast<int>(dimensions.x / 2));
		uniform_int_distribution<int> distributionY(static_cast<int>(-dimensions.y / 2), static_cast<int>(dimensions.y / 2));
		sf::Vector2f offset(static_cast<float>(distributionX(mGenerator)), static_cast<float>(distributionY(mGenerator)));
		SetTarget(mParentHive.GetCenterTarget() + offset);
	}

	sf::Vector2f newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

	mPosition = newPosition;

	PostUpdate(newPosition, rotationAngle);
}

void OnlookerBee::UpdateSeekingTarget(sf::RenderWindow& window, const double& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);

	sf::Vector2f newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);
	HandleFoodSourceCollisions();

	PostUpdate(newPosition, rotationAngle);
}

void OnlookerBee::UpdateHarvestingFood(sf::RenderWindow& window, const double& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);

	sf::Vector2f newPosition = mPosition;

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

	PostUpdate(newPosition, rotationAngle);
}

void OnlookerBee::UpdateDeliveringFood(sf::RenderWindow& window, const double& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);

	SetTarget(mParentHive.GetCenterTarget());
	sf::Vector2f newPosition = sf::Vector2f(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime,
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);
	if (DistanceBetween(newPosition, mParentHive.GetCenterTarget()) <= TARGET_RADIUS)
	{
		mState = State::DepositingFood;
	}

	PostUpdate(newPosition, rotationAngle);
}

void OnlookerBee::UpdateDepositingFood(sf::RenderWindow& window, const double& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();
	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);

	sf::Vector2f newPosition = mPosition;

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

	PostUpdate(newPosition, rotationAngle);
}

void OnlookerBee::PostUpdate(const sf::Vector2f& newPosition, const float& rotationAngle)
{
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


