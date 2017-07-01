#include "pch.h"
#include "Hive.h"
#include <cassert>


using namespace std;

Hive::Hive(const sf::Vector2f& position):
	Entity(position, sf::Color::White, sf::Color(222, 147, 12)), mDimensions(STANDARD_WIDTH, STANDARD_HEIGHT), mBody(mDimensions), 
	mFoodAmount(0.0f), mFont(), mText(), mGenerator(), mWaggleDanceClock(), mWaggleDanceWaitPeriod(2.0f), mWaggleDanceInProgress(false)
{
	std::random_device device;
	mGenerator = std::default_random_engine(device());
	mFoodSourceData.clear();
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
	mFoodSourceData.clear();
}

void Hive::update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);

	if (mWaggleDanceInProgress && mWaggleDanceClock.getElapsedTime().asSeconds() > mWaggleDanceWaitPeriod)
	{
		completeWaggleDance();
	}

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

void Hive::updateKnownFoodSource(FoodSource* const foodSource, const std::pair<float, float>& foodSourceData)
{
	mFoodSourceData[foodSource] = foodSourceData;
}

void Hive::triggerWaggleDance()
{
	mWaggleDanceClock.restart();
	mWaggleDanceInProgress = true;
}

void Hive::completeWaggleDance()
{
	std::vector<std::pair<FoodSource*, float>> fitnessWeights;
	float fitnessSum = 0.0f;

	float minYield = mFoodSourceData.begin()->second.first;
	float maxYield = mFoodSourceData.begin()->second.first;
	for (auto iter = mFoodSourceData.begin(); iter != mFoodSourceData.end(); ++iter)
	{	// Determine the range of values to help determine fitness
		auto pair = iter->second;
		if (pair.first < minYield)
		{
			minYield = pair.first;
		}
		if (pair.first > maxYield)
		{
			maxYield = pair.first;
		}
	}

	for (auto iter = mFoodSourceData.begin(); iter != mFoodSourceData.end(); ++iter)
	{
		float weight = computeFitness(iter->second, minYield, maxYield, 0.0f, 0.0f);
		std::pair<FoodSource* const, float> pair(iter->first, weight);
		fitnessWeights.push_back(pair);
		fitnessSum += weight;
	}

	for (auto iter = idleBeesBegin(); iter != idleBeesEnd(); ++iter)
	{
		assert(*iter != nullptr);

		std::uniform_real_distribution<float> distribution(0, fitnessSum);
		float roll = distribution(mGenerator);

//		float weight = 0.0f;
		for (auto fitnessIter = fitnessWeights.begin(); fitnessIter != fitnessWeights.end(); ++fitnessIter)
		{
			roll -= fitnessIter->second;
			if (roll < fitnessIter->second)
			{
				(*iter)->setTarget(fitnessIter->first);
				(*iter)->setState(Bee::State::SeekingTarget);
				break;
			}
//			weight += fitnessIter->second;
//			if (weight >= roll)
//			{	// We've reached our weighted value. send the bee
//				(*iter)->setTarget(fitnessIter->first);
//				(*iter)->setState(Bee::State::SeekingTarget);
//				break;
//			}
		}
	}

	validateIdleBees();
	mWaggleDanceInProgress = false;
}

float Hive::computeFitness(const std::pair<float, float>& foodData, 
	const float& minYield, const float& maxYield, 
	const float& minDistance, const float& maxDistance)
{
	UNREFERENCED_PARAMETER(minDistance);
	UNREFERENCED_PARAMETER(maxDistance);

	float distanceFromMinYield = foodData.first - minYield;
	float yieldRange = maxYield - minYield;

	float result = 1.0f;
	if (yieldRange != 0.0f)
	{
		result = distanceFromMinYield / yieldRange;
	}
	return result;
}
