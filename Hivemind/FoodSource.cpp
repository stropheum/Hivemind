#include "pch.h"
#include "FoodSource.h"


using namespace std;

FoodSource::FoodSource(const sf::Vector2f& position) :
	Entity(position, sf::Color::White, sf::Color(32, 128, 32)), mDimensions(STANDARD_WIDTH, STANDARD_HEIGHT), mBody(mDimensions), 
	mFoodAmount(1000.0f), mFont(), mText(), mPairedWithEmployee(false)
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

float FoodSource::takeFood(const float amount)
{
	float result = 0.0f;

	if (amount <= mFoodAmount)
	{
		mFoodAmount -= amount;
		result = amount;
	}
	else
	{
		result = mFoodAmount;
		mFoodAmount = 0.0f;
	}

	return result;
}

void FoodSource::update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);

	std::stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition.x + mBody.getSize().x / 2 - mText.getLocalBounds().width / 2, mPosition.y);
}

void FoodSource::render(sf::RenderWindow& window) const
{
	window.draw(mBody);
	window.draw(mText);
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

bool FoodSource::getPairedWithEmployee() const
{
	return mPairedWithEmployee;
}

void FoodSource::setPairedWithEmployee(const bool& pairedWithEmployee)
{
	mPairedWithEmployee = pairedWithEmployee;
}