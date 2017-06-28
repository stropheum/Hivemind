#include "pch.h"
#include "FlowField.h"
#include "Bee.h"
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "PerlinNoise.h"


using namespace std;

FlowField::FlowField(const sf::Vector2f& position) :
	Entity(position, sf::Color(100, 100, 100), sf::Color(100, 100, 100)),
	mImage(), mSprite(), mTexture(), mOctaveCount(8)
{
	mValues = new sf::Uint8*[mFieldDimensions.x];
	for (int i = 0; i < mFieldDimensions.x; i++)
	{
		mValues[i] = new sf::Uint8[mFieldDimensions.y];
		for (int j = 0; j < mFieldDimensions.y; j++)
		{
			mValues[i][j] = 0;
		}
	}

	mImage.create(mFieldDimensions.x, mFieldDimensions.y);

	generateNewField();
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

sf::Vector2i FlowField::getDimensions() const
{
	return mFieldDimensions;
}

bool FlowField::collidingWith(const sf::Vector2f& position) const
{
	return
		position.x >= mPosition.x && position.x <= mPosition.x + mFieldDimensions.x &&
		position.y >= mPosition.y && position.y <= mPosition.y + mFieldDimensions.y;
}

void FlowField::generateNewField()
{
	PerlinNoise noise;
	auto initialNoiseMap = noise.GenerateWhiteNoise(mFieldDimensions);
	auto perlinNoise = noise.generateSmoothNoise(initialNoiseMap, mFieldDimensions, mOctaveCount);

	for (int i = 0; i < mFieldDimensions.x; i++)
	{
		for (int j = 0; j < mFieldDimensions.y; j++)
		{
			mValues[i][j] = static_cast<sf::Uint8>(perlinNoise[i][j] * 255.0f);
		}
	}

	// Map the perlin noise map from 0-1 to 0-255	
	for (int i = 0; i < mFieldDimensions.x; i++)
	{
		for (int j = 0; j < mFieldDimensions.y; j++)
		{
			mImage.setPixel(j, i, sf::Color(0, mValues[i][j], mValues[i][j], 128));
		}
	}

	delete[] initialNoiseMap;
	delete[] perlinNoise;
}

void FlowField::setOctaveCount(const std::uint32_t& octaveCount)
{
	mOctaveCount = octaveCount;
}

float FlowField::radianValueAtPosition(const sf::Vector2f& position) const
{
	auto fieldPosition = position - mPosition; // Player's relative position to the flow field
	auto fieldValue = mValues[static_cast<int>(fieldPosition.x)][static_cast<int>(fieldPosition.y)];
	return static_cast<float>(fieldValue * 2.0f); // values range from 0-1, radians range from 0-2
}
