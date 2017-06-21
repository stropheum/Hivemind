#include "pch.h"
#include "Hive.h"


using namespace std;

Hive::Hive(const sf::Vector2f& position):
	Entity(position, sf::Color::White, sf::Color(222, 147, 12)), mDimensions(STANDARD_WIDTH, STANDARD_HEIGHT), mBody(mDimensions), mFoodAmount(0.0f), mFont(), mText()
{
	mBody.setPosition(mPosition);
	mBody.setOutlineThickness(5);
	mBody.setOutlineColor(mOutlineColor);
	mBody.setFillColor(mFillColor);

	if (!mFont.loadFromFile("Hack-Regular.ttf"))
	{
		throw exception("Error loading font from file");
	}

	mText.setFont(mFont);
	mText.setCharacterSize(16);
	mText.setOutlineColor(sf::Color::White);
	mText.setFillColor(sf::Color::White);
	std::stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition.x + mBody.getSize().x / 2 - mText.getLocalBounds().width / 2, mPosition.y);
}

Hive::~Hive()
{
}

void Hive::update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);

	std::stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition.x + mBody.getSize().x / 2 - mText.getLocalBounds().width / 2, mPosition.y);
}

void Hive::render(sf::RenderWindow& window) const
{
	window.draw(mBody);
	window.draw(mText);
}

sf::Vector2f Hive::getCenterTarget() const
{
	return sf::Vector2f(mPosition.x + mDimensions.x / 2, mPosition.y + mDimensions.y / 2);
}

const sf::Vector2f& Hive::getDimensions() const
{
	return mDimensions;
}

void Hive::depositFood(const float& foodAmount)
{
	mFoodAmount += foodAmount;
}
