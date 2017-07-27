#include "pch.h"
#include "HiveHUD.h"

/**
*	@Author: Dale Diaz
*	@Date: 7/21/2017
*/

using namespace std;

HiveHUD::HiveHUD(const sf::Vector2f& rootPosition, const sf::Vector2f& dimensions,
	const int& onlookerCount, const int& employeeCount, int& droneCount, const int& guardCount, const int& queenCount,
	const float& structuralComb, const float& honeyComb, const float& broodComb, const float& foodAmount):
	mOnlookerCount(onlookerCount), mEmployeeCount(employeeCount), mDroneCount(droneCount), mQueenCount(queenCount), mGuardCount(guardCount),
	mStructuralComb(structuralComb), mHoneyComb(honeyComb), mBroodComb(broodComb),
	mRootPosition(rootPosition), mDimensions(dimensions), mFoodAmount(foodAmount),
	mOutlineThickness(4)
{
	mBeeContainer.setFillColor(sf::Color::Transparent);
	mBeeContainer.setOutlineColor(sf::Color(196, 196, 196));
	mBeeContainer.setOutlineThickness(mOutlineThickness);

	mCombContainer.setFillColor(sf::Color::Transparent);
	mCombContainer.setOutlineColor(sf::Color(196, 196, 196));
	mCombContainer.setOutlineThickness(mOutlineThickness);

	mFoodContainer.setFillColor(sf::Color::Transparent);
	mFoodContainer.setOutlineColor(sf::Color(196, 196, 196));
	mFoodContainer.setOutlineThickness(mOutlineThickness);

	mBarOnlookers.setFillColor(sf::Color(255, 204, 0));
	mBarEmployees.setFillColor(sf::Color::Cyan);
	mBarDrones.setFillColor(sf::Color(128, 128, 128));
	mBarGuards.setFillColor(sf::Color(128, 0, 0));
	mBarQueens.setFillColor(sf::Color::Magenta);

	mBarStructuralComb.setFillColor(sf::Color(225, 225, 225));
	mBarHoneyComb.setFillColor(sf::Color::Green);
	mBarBroodComb.setFillColor(sf::Color(255, 164, 0));
	mBarFoodAmount.setFillColor(sf::Color::Green);

	mBarOnlookers.setOutlineColor(sf::Color::Transparent);
	mBarEmployees.setOutlineColor(sf::Color::Transparent);
	mBarGuards.setOutlineColor(sf::Color::Transparent);
	mBarDrones.setOutlineColor(sf::Color::Transparent);
	mBarQueens.setOutlineColor(sf::Color::Transparent);
	mBarStructuralComb.setOutlineColor(sf::Color::Transparent);
	mBarHoneyComb.setOutlineColor(sf::Color::Transparent);
	mBarBroodComb.setOutlineColor(sf::Color::Transparent);
	mBarFoodAmount.setOutlineColor(sf::Color::Transparent);
	UpdateHUDValues();
}

void HiveHUD::Render(sf::RenderWindow& window) const
{
	window.draw(mBarOnlookers);
	window.draw(mBarEmployees);
	window.draw(mBarDrones);
	window.draw(mBarGuards);
	window.draw(mBarQueens);

	window.draw(mBarStructuralComb);
	window.draw(mBarHoneyComb);
	window.draw(mBarBroodComb);

	window.draw(mBarFoodAmount);

	window.draw(mBeeContainer);
	window.draw(mCombContainer);
	window.draw(mFoodContainer);
}

void HiveHUD::UpdateHUDValues()
{
	float beeSum = mOnlookerCount + mEmployeeCount + mDroneCount + mGuardCount + mQueenCount;

	mBeeContainer.setPosition(mRootPosition);
	mBeeContainer.setSize(mDimensions);

	mBarOnlookers.setPosition(mRootPosition);
	mBarOnlookers.setSize(sf::Vector2f(mDimensions.x * (mOnlookerCount / beeSum), mDimensions.y));

	mBarEmployees.setPosition(sf::Vector2f(mBarOnlookers.getPosition().x + mBarOnlookers.getSize().x, mRootPosition.y));
	mBarEmployees.setSize(sf::Vector2f(mDimensions.x * (mEmployeeCount / beeSum), mDimensions.y));

	mBarDrones.setPosition(sf::Vector2f(mBarEmployees.getPosition().x + mBarEmployees.getSize().x, mRootPosition.y));
	mBarDrones.setSize(sf::Vector2f(mDimensions.x * (mDroneCount / beeSum), mDimensions.y));

	mBarGuards.setPosition(sf::Vector2f(mBarDrones.getPosition().x + mBarDrones.getSize().x, mRootPosition.y));
	mBarGuards.setSize(sf::Vector2f(mDimensions.x * (mGuardCount / beeSum), mDimensions.y));

	mBarQueens.setPosition(sf::Vector2f(mBarGuards.getPosition().x + mBarGuards.getSize().x, mRootPosition.y));
	mBarQueens.setSize(sf::Vector2f(mDimensions.x * (mQueenCount / beeSum), mDimensions.y));

	float combSum = mStructuralComb + mHoneyComb + mBroodComb;

	mCombContainer.setPosition(mRootPosition + sf::Vector2f(0, mDimensions.y + mOutlineThickness));
	mCombContainer.setSize(mDimensions);

	mBarHoneyComb.setPosition(mCombContainer.getPosition());
	mBarHoneyComb.setSize(sf::Vector2f(mDimensions.x * (mHoneyComb / combSum), mDimensions.y));

	mBarBroodComb.setPosition(mBarHoneyComb.getPosition() + sf::Vector2f(mBarHoneyComb.getSize().x, 0));
	mBarBroodComb.setSize(sf::Vector2f(mDimensions.x * (mBroodComb / combSum), mDimensions.y));

	mBarStructuralComb.setPosition(mBarBroodComb.getPosition() + sf::Vector2f(mBarBroodComb.getSize().x, 0));
	mBarStructuralComb.setSize(sf::Vector2f(mDimensions.x * (mStructuralComb / combSum), mDimensions.y));

	mFoodContainer.setPosition(mCombContainer.getPosition() + sf::Vector2f(0, mDimensions.y + mOutlineThickness));
	mFoodContainer.setSize(mDimensions);

	mBarFoodAmount.setPosition(mFoodContainer.getPosition());
	float barPercentage = mFoodAmount / mHoneyComb;
	if (barPercentage > 1.0f)
	{	// Clamp food amount
		barPercentage = 1.0f;
	}
	mBarFoodAmount.setSize(sf::Vector2f(mBarHoneyComb.getSize().x * barPercentage, mDimensions.y));
}
