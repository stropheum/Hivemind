#include "pch.h"
#include "Bee.h"
#include <math.h>
#include "BeeManager.h"
#include "FoodSource.h"
#include <winnt.h>


const float Bee::STANDARD_BEE_SPEED = 200.0f;
const float Bee::BODY_RADIUS = 12.0f;
const sf::Color Bee::NORMAL_COLOR = sf::Color(192, 192, 192);
const sf::Color Bee::ALERT_COLOR = sf::Color::Red;
const sf::Color Bee::STANDARD_BODY_COLOR = sf::Color(255, 204, 0);

Bee::Bee(const sf::Vector2f& position):
	Entity(position, NORMAL_COLOR, STANDARD_BODY_COLOR), mBody(BODY_RADIUS), mFace(sf::Vector2f(BODY_RADIUS, 2)), 
	mTarget(position), mSpeed(STANDARD_BEE_SPEED), mTargeting(false)
{
	mBody.setFillColor(mFillColor);
	mBody.setOutlineColor(mOutlineColor);
	mBody.setOutlineThickness(3);
	mFace.setFillColor(sf::Color::White);
	mBody.setPosition(sf::Vector2f(mPosition.x - BODY_RADIUS, mPosition.y - BODY_RADIUS));
	mFace.setPosition(mBody.getPosition().x, mBody.getPosition().y + BODY_RADIUS);
}

void Bee::update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	auto facePosition = mFace.getPosition();

	float rotationRadians = atan2(mTarget.y - facePosition.y, mTarget.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / PI);

	sf::Vector2f newPosition(
		mPosition.x + cos(rotationRadians) * mSpeed * deltaTime, 
		mPosition.y + sin(rotationRadians) * mSpeed * deltaTime);

	mPosition = newPosition;
	mBody.setPosition(sf::Vector2f(mPosition.x - BODY_RADIUS, mPosition.y - BODY_RADIUS));
	mFace.setPosition(mPosition.x, mPosition.y);
	mFace.setRotation(rotationAngle);
}

void Bee::render(sf::RenderWindow& window) const
{
	window.draw(mBody);
	window.draw(mFace);
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
