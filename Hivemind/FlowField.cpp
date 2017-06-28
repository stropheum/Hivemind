#include "pch.h"
#include "FlowField.h"
#include "Bee.h"
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>


using namespace std;

FlowField::FlowField(const sf::Vector2f& position) :
	Entity(position, sf::Color(100, 100, 100), sf::Color(100, 100, 100)), 
	mImage(), mSprite(), mTexture()
{
	mValues = new sf::Vector2f*[mFieldDimensions.x];
	for (int i = 0; i < mFieldDimensions.x; i++)
	{
		mValues[i] = new sf::Vector2f[mFieldDimensions.y];
		for (int j = 0; j < mFieldDimensions.y; j++)
		{
			mValues[i][j] = sf::Vector2f(0, 0);
		}
	}

	mImage.create(mFieldDimensions.x, mFieldDimensions.y);
	
	for (int i = 0; i < mFieldDimensions.x; i++)
	{
		for (int j = 0; j < mFieldDimensions.y; j++)
		{	// Temporarily initialize the pixel array as cyan for debugging
			mImage.setPixel(j, i, sf::Color(0, static_cast<sf::Uint8>(mValues[i][j].x), static_cast<sf::Uint8>(mValues[i][j].y), 128));
		}
	}
}

FlowField::~FlowField()
{
	for (int i = 0; i < mFieldDimensions.x; i++)
	{
		delete mValues[i];
	}
	delete mValues;
}

void FlowField::update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);

	mTexture.loadFromImage(mImage);
	mSprite.setPosition(mPosition);
	mSprite.setTexture(mTexture);
}

void FlowField::render(sf::RenderWindow& window) const
{
	window.draw(mSprite);
}

sf::Vector2i FlowField::getDimensions()
{
	return mFieldDimensions;
}

bool FlowField::collidingWith(const sf::Vector2f& position) const
{
	return
		position.x >= mPosition.x && position.x <= mPosition.x + mFieldDimensions.x &&
		position.y >= mPosition.y && position.y <= mPosition.y + mFieldDimensions.y;
}
