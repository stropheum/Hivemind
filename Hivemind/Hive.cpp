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
	mIdleBees.clear();
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

void Hive::addIdleBee(OnlookerBee* const bee)
{
	bool containsBee = false;

	for (auto iter = mIdleBees.begin(); iter != mIdleBees.end(); ++iter)
	{
		if (*iter == bee)
		{
			containsBee = true;
			break;
		}
	}

	if (!containsBee)
	{	// Only add the bee if it does not exist in the collection already
		mIdleBees.push_back(bee);
	}
}

void Hive::removeIdleBee(OnlookerBee* const bee)
{
	for (auto iter = mIdleBees.begin(); iter != mIdleBees.end(); ++iter)
	{
		if (*iter == bee)
		{	// If we encounter the bee, remove it. If not, no behavior
			mIdleBees.erase(iter);
			break;
		}
	}
}

std::vector<OnlookerBee*>::iterator Hive::idleBeesBegin()
{
	return mIdleBees.begin();
}

std::vector<OnlookerBee*>::iterator Hive::idleBeesEnd()
{
	return mIdleBees.end();
}

void Hive::validateIdleBees()
{
	bool beeRemoved = true;
	while (beeRemoved)
	{	// Repeat until we look through all bees and all are idle
		beeRemoved = false;
		for (auto iter = mIdleBees.begin(); iter != mIdleBees.end(); ++iter)
		{
			if ((*iter)->getState() != Bee::State::Idle)
			{	// We know it isn't idle, so remove it
				beeRemoved = true;
				mIdleBees.erase(iter);
				break;
			}
		}
	}
}
