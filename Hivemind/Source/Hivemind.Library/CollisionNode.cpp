#include "pch.h"
#include "CollisionNode.h"


using namespace std;

CollisionNode::CollisionNode():
	Entity(sf::Vector2f(), sf::Color(255, 255, 255, 32), sf::Color::Transparent), mSize(0)
{
	mBorder.setOutlineThickness(-40);
	mBorder.setOutlineColor(mOutlineColor);
	mBorder.setFillColor(mFillColor);

	mText.setFont(FontManager::GetInstance()->Hack());
	mText.setCharacterSize(64);
	mText.setOutlineColor(sf::Color::White);
	mText.setFillColor(sf::Color::White);
	UpdateTextDisplay();
}

CollisionNode::CollisionNode(const sf::Vector2f& position, const int& size):
	Entity(position, sf::Color::White, sf::Color::Transparent), mSize(size)
{
	mBorder.setSize(sf::Vector2f(size, size));
	mBorder.setOutlineThickness(-4);
	mBorder.setOutlineColor(mOutlineColor);
	mBorder.setFillColor(mFillColor);

	mText.setFont(FontManager::GetInstance()->Hack());
	mText.setCharacterSize(16);
	mText.setOutlineColor(sf::Color::White);
	mText.setFillColor(sf::Color::White);
	UpdateTextDisplay();
}

CollisionNode::~CollisionNode()
{
}

void CollisionNode::Update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);
	UpdateTextDisplay();
}

void CollisionNode::Render(sf::RenderWindow& window) const
{
	window.draw(mBorder);
	window.draw(mText);
}

void CollisionNode::SetPosition(const sf::Vector2f& position)
{
	Entity::SetPosition(position);
	mBorder.setPosition(position);
	mText.setPosition(mPosition + sf::Vector2f(50, 50));
}

void CollisionNode::SetSize(const int& size)
{
	mSize = size;
	mBorder.setSize(sf::Vector2f(mSize, mSize));
}

int CollisionNode::GetSize() const
{
	return mSize;
}

void CollisionNode::RegisterHive(Hive* const hive)
{
	mHives.push_back(hive);
	UpdateTextDisplay();
}

void CollisionNode::UnregisterHive(Hive* const hive)
{
	for (auto iter = mHives.begin(); iter != mHives.end(); ++iter)
	{
		if (*iter == hive)
		{
			mHives.erase(iter);
			break;
		}
	}
	UpdateTextDisplay();
}

void CollisionNode::RegisterFoodSource(FoodSource* const foodSource)
{
	mFoodSources.push_back(foodSource);
	UpdateTextDisplay();
}

void CollisionNode::UnregisterFoodSource(FoodSource* const foodSource)
{
	for (auto iter = mFoodSources.begin(); iter != mFoodSources.end(); ++iter)
	{
		if (*iter == foodSource)
		{
			mFoodSources.erase(iter);
			break;
		}
	}
	UpdateTextDisplay();
}

void CollisionNode::RegisterBee(Bee* const bee)
{
	mBees.push_back(bee);
	UpdateTextDisplay();
}

void CollisionNode::UnregisterBee(Bee* const bee)
{
	for (auto iter = mBees.begin(); iter != mBees.end(); ++iter)
	{
		if (*iter == bee)
		{
			mBees.erase(iter);
			break;
		}
	}
	UpdateTextDisplay();
}

bool CollisionNode::ContainsPoint(const sf::Vector2f& point) const
{
	return
		point.x >= mPosition.x &&
		point.x < (mPosition.x + mSize) &&
		point.y >= mPosition.y &&
		point.y < (mPosition.y + mSize);
}

std::vector<Hive*> CollisionNode::Hives() const
{
	return mHives;
}

std::vector<FoodSource*> CollisionNode::FoodSources() const
{
	return mFoodSources;
}

std::vector<Bee*> CollisionNode::Bees() const
{
	return mBees;
}

void CollisionNode::UpdateTextDisplay()
{
	stringstream ss;
	ss << "Hives: " << mHives.size() << endl;
	ss << "Food Sources: " << mFoodSources.size() << endl;
	ss << "Bees: " << mBees.size() << endl;
	mText.setString(ss.str());
}
