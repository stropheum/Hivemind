#include "pch.h"
#include "Hive.h"
#include <sstream>


Hive::Hive(const sf::Vector2f& position): 
	Entity(position, sf::Color::White, sf::Color(211, 163, 8)), mDimensions(STANDARD_WIDTH, STANDARD_HEIGHT), mBody(mDimensions), mFoodAmount(0.0f), mText(), mFont()
{
	mBody.setPosition(mPosition);
	mBody.setOutlineThickness(5);
	mBody.setOutlineColor(mOutlineColor);
	mBody.setFillColor(mFillColor);

	if (!mFont.loadFromFile("Hack-Regular.ttf"))
	{
		throw std::exception("Error loading font from file");
	}
	mText.setFont(mFont);
	mText.setCharacterSize(16);
	mText.setOutlineColor(sf::Color::White);
	mText.setFillColor(sf::Color::White);
	std::stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(position);
}

void Hive::update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);
	mText.setPosition(mPosition);
	mBody.setPosition(mPosition);
}

void Hive::render(sf::RenderWindow& window) const
{
	window.draw(mBody);
	window.draw(mText);
}
