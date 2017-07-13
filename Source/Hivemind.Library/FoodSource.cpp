#include "pch.h"
#include "FoodSource.h"


using namespace std;

float FoodSource::DetectionRadius = 400.0f;

FoodSource::FoodSource(const sf::Vector2f& position) :
	Entity(position, sf::Color::White, sf::Color(32, 128, 32)), mDimensions(STANDARD_WIDTH, STANDARD_HEIGHT), mBody(mDimensions),
	mFoodAmount(10000.0f), mText(), mPairedWithEmployee(false)
{
	mBody.setPosition(mPosition);
	mBody.setOutlineThickness(-2);
	mBody.setOutlineColor(mOutlineColor);
	mBody.setFillColor(mFillColor);

	mText.setFont(FontManager::GetInstance()->Hack());
	mText.setCharacterSize(16);
	mText.setOutlineColor(sf::Color::White);
	mText.setFillColor(sf::Color::White);
	std::stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition.x + mBody.getSize().x / 2 - mText.getLocalBounds().width / 2, mPosition.y);
}

float FoodSource::TakeFood(const float amount)
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

bool FoodSource::IsViable() const
{
	return mFoodAmount > 0.0f;
}

void FoodSource::Update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);

	if (mCollisionNode != nullptr && !mCollisionNode->ContainsPoint(mPosition))
	{	// If we haev a collision node and we leave it, invalidate the pointer
		mCollisionNode->UnregisterFoodSource(this);
		mCollisionNode = nullptr;
	}

	if (mCollisionNode == nullptr)
	{	// If the collision node is invalidated, get a new one and register to it
		mCollisionNode = CollisionGrid::GetInstance()->CollisionNodeFromPosition(mPosition);
		mCollisionNode->RegisterFoodSource(this);
	}

	std::stringstream ss;
	ss << "Food: " << mFoodAmount;
	mText.setString(ss.str());
	mText.setPosition(mPosition.x + mBody.getSize().x / 2 - mText.getLocalBounds().width / 2, mPosition.y);
}

void FoodSource::Render(sf::RenderWindow& window) const
{
	window.draw(mBody);
	window.draw(mText);
}

float FoodSource::GetFoodAmount() const
{
	return mFoodAmount;
}

void FoodSource::SetFoodAmount(const float& foodAmount)
{
	mFoodAmount = foodAmount;
}

sf::Vector2f FoodSource::GetCenterTarget() const
{
	return sf::Vector2f(mPosition.x + mDimensions.x / 2, mPosition.y + mDimensions.y / 2);
}

const sf::Vector2f& FoodSource::GetDimensions() const
{
	return mDimensions;
}

bool FoodSource::PairedWithEmployee() const
{
	return mPairedWithEmployee;
}

void FoodSource::SetPairedWithEmployee(const bool& pairedWithEmployee)
{
	mPairedWithEmployee = pairedWithEmployee;
}