#include "pch.h"
#include "FoodSource.h"
#include <winnt.h>


FoodSource::FoodSource(const sf::Vector2f& position): 
	mDimensions(STANDARD_WIDTH, STANDARD_HEIGHT), mBody(mDimensions), mFoodAmount(100.0f)
{
	mBody.setPosition(position);
	mBody.setOutlineThickness(5);
	mBody.setOutlineColor(sf::Color::White);
	mBody.setFillColor(sf::Color::Green);
}

void FoodSource::update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);
}

void FoodSource::render(sf::RenderWindow& window) const
{
	window.draw(mBody);
}

float FoodSource::getFoodAmount() const
{
	return mFoodAmount;
}

void FoodSource::setFoodAmount(const float& foodAmount)
{
	mFoodAmount = foodAmount;
}

const sf::Vector2f& FoodSource::getPosition() const
{
	return mBody.getPosition();
}

const sf::Vector2f FoodSource::getCenterTarget() const
{
	return sf::Vector2f(mBody.getPosition().x + mDimensions.x / 2, mBody.getPosition().y + mDimensions.y / 2);
}

const sf::Vector2f& FoodSource::getDimensions() const
{
	return mDimensions;
}
