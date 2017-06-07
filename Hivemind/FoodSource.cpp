#include "pch.h"
#include "FoodSource.h"
#include <winnt.h>


FoodSource::FoodSource(const sf::Vector2f& position): 
	Entity(position, sf::Color::White, sf::Color(32, 128, 32)), mDimensions(STANDARD_WIDTH, STANDARD_HEIGHT), mBody(mDimensions), mFoodAmount(100.0f)
{
	mBody.setPosition(mPosition);
	mBody.setOutlineThickness(5);
	mBody.setOutlineColor(mOutlineColor);
	mBody.setFillColor(mFillColor);
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

sf::Vector2f FoodSource::getCenterTarget() const
{
	return sf::Vector2f(mPosition.x + mDimensions.x / 2, mPosition.y + mDimensions.y / 2);
}

const sf::Vector2f& FoodSource::getDimensions() const
{
	return mDimensions;
}
