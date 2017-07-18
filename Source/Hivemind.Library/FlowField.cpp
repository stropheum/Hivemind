#include "pch.h"
#include "FlowField.h"
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "PerlinNoise.h"


using namespace std;

FlowField::FlowField(const sf::Vector2f& position):
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
	GenerateNewField();
}

FlowField::~FlowField()
{
	for (int i = 0; i < mFieldDimensions.x; i++)
	{
		if (mValues[i] != nullptr)
		{
			delete[] mValues[i];
		}
	}
	if (mValues != nullptr)
	{
		delete[] mValues;
	}
}

FlowField::FlowField(const FlowField& rhs):
	Entity(rhs), mImage(), mSprite(), mTexture(), mOctaveCount(rhs.mOctaveCount)
{
	mValues = new sf::Uint8*[mFieldDimensions.x];
	for (int i = 0; i < mFieldDimensions.x; i++)
	{
		mValues[i] = new sf::Uint8[mFieldDimensions.y];
		for (int j = 0; j < mFieldDimensions.y; j++)
		{
			mValues[i][j] = rhs.mValues[i][j];
		}
	}

	mImage = rhs.mImage;
}

FlowField& FlowField::operator=(const FlowField& rhs)
{
	mPosition = rhs.mPosition;

	mValues = new sf::Uint8*[mFieldDimensions.x];
	for (int i = 0; i < mFieldDimensions.x; i++)
	{
		mValues[i] = new sf::Uint8[mFieldDimensions.y];
		for (int j = 0; j < mFieldDimensions.y; j++)
		{
			mValues[i][j] = rhs.mValues[i][j];
		}
	}

	mImage = rhs.mImage;

	return (*this);
}

void FlowField::Update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);
	
}

void FlowField::Render(sf::RenderWindow& window) const
{
	window.draw(mSprite);
}

sf::Vector2i FlowField::GetDimensions() const
{
	return mFieldDimensions;
}

bool FlowField::CollidingWith(const sf::Vector2f& position) const
{
	return
		position.x >= mPosition.x && position.x < mPosition.x + mFieldDimensions.x &&
		position.y >= mPosition.y && position.y < mPosition.y + mFieldDimensions.y;
}

void FlowField::GenerateNewField()
{
	PerlinNoise noise;
	auto initialNoiseMap = noise.GenerateWhiteNoise(mFieldDimensions);
	auto perlinNoise = noise.GeneratePerlinNoise(initialNoiseMap, mFieldDimensions, mOctaveCount);

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
			mImage.setPixel(i, j, sf::Color(0, mValues[i][j], mValues[i][j], 128));
		}
	}

	for (int i = 0; i < mFieldDimensions.x; i++)
	{
		delete[] initialNoiseMap[i];
	}
	delete[] initialNoiseMap;
	for (int i = 0; i < mFieldDimensions.x; i++)
	{
		delete[] perlinNoise[i];
	}
	delete[] perlinNoise;
}

void FlowField::SetOctaveCount(const std::uint32_t& octaveCount)
{
	mOctaveCount = octaveCount;
}

float FlowField::RadianValueAtPosition(const sf::Vector2f& position) const
{
	sf::Vector2f fieldPosition = position - mPosition; // Player's relative position to the flow field
	float fieldValue = mValues[static_cast<int>(fieldPosition.x)][static_cast<int>(fieldPosition.y)];
	return static_cast<float>(fieldValue * 2.0f); // values range from 0-1, radians range from 0-2
}

void FlowField::SetPosition(const sf::Vector2f& position)
{
	Entity::SetPosition(position);
	mSprite.setPosition(mPosition);
	mTexture.loadFromImage(mImage);
	mTexture.setSmooth(true);
	mSprite.setTexture(mTexture);
}
