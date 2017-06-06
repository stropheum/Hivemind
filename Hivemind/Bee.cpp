#include "pch.h"
#include "Bee.h"
#include <math.h>
#include "BeeManager.h"
#include "FoodSource.h"


const float Bee::STANDARD_BEE_SPEED = 20.0f;
const float Bee::BODY_RADIUS = 5.0f;
const sf::Color Bee::NORMAL_COLOR = sf::Color(200, 200, 200);
const sf::Color Bee::ALERT_COLOR = sf::Color::Red;

Bee::Bee(const sf::Vector2f& position):
	mBody(BODY_RADIUS), mFace(sf::Vector2f(BODY_RADIUS, 2)), mPosition(position), mTarget(position), mSpeed(STANDARD_BEE_SPEED), mTargeting(false)
{
	mBody.setFillColor(sf::Color(0, 128, 128));
	mBody.setOutlineColor(NORMAL_COLOR);
	mBody.setOutlineThickness(2);
	mFace.setFillColor(sf::Color::White);
	mBody.setPosition(sf::Vector2f(mPosition.x - BODY_RADIUS, mPosition.y - BODY_RADIUS));
	mFace.setPosition(mBody.getPosition().x, mBody.getPosition().y + BODY_RADIUS);
}

void Bee::update(sf::RenderWindow& window, const float& deltaTime)
{
	auto facePosition = mFace.getPosition();

	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);
	
	auto xDif = abs(mTarget.x - facePosition.x);
	auto yDif = abs(mTarget.y - facePosition.y);
	auto distance = sqrt(xDif * xDif + yDif * yDif);

	sf::Vector2f newPosition(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime, 
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);
	bool validPosition = true;
	auto beeManager = BeeManager::getInstance();

	if (validPosition)
	{
		mPosition = newPosition;
		mBody.setPosition(sf::Vector2f(mPosition.x - BODY_RADIUS, mPosition.y - BODY_RADIUS));
		mFace.setPosition(mPosition.x, mPosition.y);
	}

	mFace.setRotation(rotationAngle);
}

void Bee::render(sf::RenderWindow& window) const
{
	window.draw(mBody);
	window.draw(mFace);
}

void Bee::setPosition(const sf::Vector2f& position)
{
	mPosition = position;
}

const sf::Vector2f& Bee::getPosition() const
{
	return mPosition;
}

float Bee::distanceBetween(const sf::Vector2f& position_1, const sf::Vector2f& position_2) const
{
	auto xDif = abs(position_1.x - position_2.x);
	auto yDif = abs(position_1.y - position_2.y);
	return sqrt((xDif * xDif) + (yDif * yDif));
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
