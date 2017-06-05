#include "pch.h"
#include "Bee.h"
#include <math.h>
#include "BeeManager.h"


Bee::Bee():
	mBody(mBodyRadius), mFace(sf::Vector2f(mBodyRadius, 2)), mPosition(sf::Vector2f(200, 200)), speed(1.0f)
{
	mBody.setFillColor(sf::Color(0, 128, 128));
	mFace.setFillColor(sf::Color::Black);
	mBody.setPosition(mPosition);
	mFace.setPosition(mBody.getPosition().x + mBodyRadius, mBody.getPosition().y + mBodyRadius);
}

Bee::Bee(const sf::Vector2f& position):
	mBody(mBodyRadius), mFace(sf::Vector2f(mBodyRadius, 2)), mPosition(position), speed(0.01f)
{
	mBody.setFillColor(sf::Color(0, 128, 128));
	mFace.setFillColor(sf::Color::Black);
	mBody.setPosition(mPosition);
	mFace.setPosition(mBody.getPosition().x + mBodyRadius, mBody.getPosition().y + mBodyRadius);
}

void Bee::update(sf::RenderWindow& window, const float& deltaTime)
{
	mBody.setPosition(mPosition);
	mFace.setPosition(mPosition.x + mBodyRadius, mPosition.y + mBodyRadius);

	auto mousePosition = sf::Mouse::getPosition(window);
	auto facePosition = mFace.getPosition();

	float rotationRadians = atan2(mousePosition.y - facePosition.y, mousePosition.x - facePosition.x);
	float rotationAngle = rotationRadians * (180 / pi);
	
	auto xDif = abs(mousePosition.x - facePosition.x);
	auto yDif = abs(mousePosition.y - facePosition.y);
	auto distance = sqrt(xDif * xDif + yDif * yDif);

	sf::Vector2f newPosition(
		mPosition.x + cos(rotationRadians) * speed * deltaTime, 
		mPosition.y + sin(rotationRadians) * speed * deltaTime);
	bool validPosition = true;
	auto beeManager = BeeManager::getInstance();
	
	for (auto iter = beeManager->begin(); iter != beeManager->end(); ++iter)
	{
		if (&(*iter) != this)
		{	// Disregard checking identical bees
			float beeDistance = distanceBetween(newPosition, iter->getPosition());
			if (beeDistance < (mBodyRadius + iter->getRadius()))
			{
				validPosition = false;
				break;
			}
		}
	}

	if (validPosition)
	{
		mPosition = newPosition;
	}

//	mPosition.x += cos(rotationRadians) * speed;
//	mPosition.y += sin(rotationRadians) * speed;

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

float Bee::getRadius() const
{
	return mBodyRadius;
}

float Bee::distanceBetween(const sf::Vector2f& position_1, const sf::Vector2f& position_2) const
{
	auto xDif = abs(position_1.x - position_2.x);
	auto yDif = abs(position_1.y - position_2.y);
	return sqrt((xDif * xDif) + (yDif * yDif));
}