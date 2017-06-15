#include "pch.h"
#include "Bee.h"
#include "BeeManager.h"
#include "FoodSource.h"
#include "FoodSourceManager.h"
#include <sstream>

using namespace std;
using namespace std::chrono;


const float Bee::STANDARD_BEE_SPEED = 200.0f;
const float Bee::BODY_RADIUS = 12.0f;
const float Bee::TARGET_RADIUS = 5.0f;
const float Bee::STANDARD_HARVESTING_DURATION = 10.0f;
const sf::Color Bee::NORMAL_COLOR = sf::Color(192, 192, 192);
const sf::Color Bee::ALERT_COLOR = sf::Color::Red;
const sf::Color Bee::STANDARD_BODY_COLOR = sf::Color(255, 204, 0);

Bee::Bee(const sf::Vector2f& position):
	Entity(position, NORMAL_COLOR, STANDARD_BODY_COLOR), mGenerator(), mBody(BODY_RADIUS), mFace(sf::Vector2f(BODY_RADIUS, 2)), 
	mTarget(position), mSpeed(STANDARD_BEE_SPEED), mTargeting(false), mState(State::SeekingTarget),
	mHarvestingDuration(STANDARD_HARVESTING_DURATION), mHarvestingClock(), mTargetFoodSource(nullptr), mFoodAmount(0.0f), mText(), mFont(nullptr)
{
	mGenerator.seed(static_cast<long>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	mBody.setFillColor(mFillColor);
	mBody.setOutlineColor(mOutlineColor);
	mBody.setOutlineThickness(3);
	mFace.setFillColor(sf::Color::White);
	mBody.setPosition(sf::Vector2f(mPosition.x - BODY_RADIUS, mPosition.y - BODY_RADIUS));
	mFace.setPosition(mBody.getPosition().x, mBody.getPosition().y + BODY_RADIUS);

	mText.setCharacterSize(16);
	mText.setOutlineColor(sf::Color::White);
	mText.setFillColor(sf::Color::White);
	stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition - sf::Vector2f(mText.getLocalBounds().width/2.0f, 35));
}

void Bee::update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();

	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);

	sf::Vector2f newPosition;
	switch(mState)
	{
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
			mState = State::SeekingTarget;
		}

		setColor(Bee::NORMAL_COLOR);
		for (auto iter = FoodSourceManager::getInstance()->begin(); iter != FoodSourceManager::getInstance()->end(); ++iter)
		{
			if (collidingWithFoodSource(*iter))
			{
				setColor(Bee::ALERT_COLOR);
				break;
			}
		}
		break;
	}

	mPosition = newPosition;
	mBody.setPosition(sf::Vector2f(mPosition.x - BODY_RADIUS, mPosition.y - BODY_RADIUS));
	mFace.setPosition(mPosition.x, mPosition.y);
	mFace.setRotation(rotationAngle);

	stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition - sf::Vector2f(mText.getLocalBounds().width / 2.0f, 35));
}

void Bee::render(sf::RenderWindow& window) const
{
	window.draw(mBody);
	window.draw(mFace);
	if (mFont != nullptr)
	{
		window.draw(mText);
	}
}

bool Bee::hasTarget() const
{
	return mTargeting;
}

void Bee::setFont(sf::Font* const font)
{
	mFont = font;
	mText.setFont(*font);
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

void Bee::setColor(const sf::Color& color)
{
	mBody.setOutlineColor(color);
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
	bool colliding = false;
	bool reachedCenterOfSource = false;
	
	if (!hasTarget())
	{	// Set initial target
		std::uniform_int_distribution<int> distribution(0, foodSourceManager->getFoodsourceCount() - 1);
		int targetIndex = distribution(mGenerator);
		mTargetFoodSource = &foodSourceManager->getFoodSource(targetIndex);
		sf::Vector2f newTarget = mTargetFoodSource->getCenterTarget();

		setTarget(newTarget);
	}

	for (auto foodIter = foodSourceManager->begin(); foodIter != foodSourceManager->end(); ++foodIter)
	{
		if (collidingWithFoodSource(*foodIter))
		{
			colliding = true;
			if (Entity::distanceBetween(getPosition(), getTarget()) <= Bee::TARGET_RADIUS)
			{
				reachedCenterOfSource = true;
				sf::Vector2f newTarget;
				do
				{
					std::uniform_int_distribution<int> distribution(0, foodSourceManager->getFoodsourceCount() - 1);
					int targetIndex = distribution(mGenerator);
					mTargetFoodSource = &foodSourceManager->getFoodSource(targetIndex);
					newTarget = mTargetFoodSource->getCenterTarget();
				} while (newTarget == getTarget());

				setTarget(newTarget);
			}
		}
	}

	setColor(colliding ? Bee::ALERT_COLOR : Bee::NORMAL_COLOR);
	if (reachedCenterOfSource)
	{
		mState = State::HarvestingFood;
		mHarvestingClock.restart();
	}
}
