#include "pch.h"
#include "FoodSource.h"


FoodSource::FoodSource(const sf::Vector2f& position): 
	mWidth(STANDARD_WIDTH), mHeight(STANDARD_HEIGHT), mBody(sf::Vector2f(mWidth, mHeight)), mFoodAmount(100.0f)
{
	mBody.setPosition(position);
	mBody.setOutlineThickness(5);
	mBody.setOutlineColor(sf::Color::White);
	mBody.setFillColor(sf::Color::Green);
}

void FoodSource::update(sf::RenderWindow& window, const float& deltaTime)
{
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