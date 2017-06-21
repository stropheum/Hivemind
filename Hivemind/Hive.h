#pragma once
#include "Entity.h"
class Hive :
	public Entity
{
public:
	/// Constructor
	/// @Param position: The starting position of the food source
	explicit Hive(const sf::Vector2f& position);
	virtual ~Hive();

	void update(sf::RenderWindow& window, const float& deltaTime) override;
	void render(sf::RenderWindow& window) const override;

	/// Accessor method for the center point of the food source
	/// @Return: A vector representing the center point of the source
	sf::Vector2f getCenterTarget() const;

	/// Accessor method for the dimensions of the food source
	/// @Return: A vector representing the width and height of the food source
	const sf::Vector2f& getDimensions() const;

	void depositFood(const float& foodAmount);

private:
	/// Constants
	const float STANDARD_WIDTH = 200.0f;
	const float STANDARD_HEIGHT = 200.0f;

	/// Fields
	sf::Vector2f mDimensions;
	sf::RectangleShape mBody;
	float mFoodAmount;
	sf::Font mFont;
	sf::Text mText;
};

