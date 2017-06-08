#pragma once
#include "Entity.h"

class FoodSource : public Entity
{
public:
	
#pragma region Constructors / Destructor / Move semantics

	/// Constructor
	/// @Param position: The starting position of the food source
	explicit FoodSource(const sf::Vector2f& position);

	/// Default Destructor
	~FoodSource() = default;

	/// Default Copy Constructor
	/// @Param rhs: The food source being copied
	FoodSource(const FoodSource& rhs) = default;

	/// Default Assignment Operator
	/// @Param rhs: The food source being assigned to
	/// @Return: A copy of the specified food source
	FoodSource& operator=(const FoodSource& rhs) = default;

	/// Default Move Copy Constructor
	/// @Param rhs: The food source being moved
	FoodSource(FoodSource&& rhs) = default;

	/// Default Move Assignment Operator
	/// @Param rhs: The food source being moved
	/// @Return: A reference to the moved food source
	FoodSource& operator=(FoodSource&& rhs) = default;

#pragma endregion 

#pragma region Public API

	//TODO: Move update and render to Entity
	void update(sf::RenderWindow& window, const float& deltaTime);
	void render(sf::RenderWindow& window) const;

	/// Accessor for the amount of food that is currently stored
	/// @Return: The amount of food stored in the source
	float getFoodAmount() const;

	/// Mutator method for the food amount in the source
	/// @Param foodAmount: The new amount of food in the source
	void setFoodAmount(const float& foodAmount);

	/// Accessor method for the center point of the food source
	/// @Return: A vector representing the center point of the source
	sf::Vector2f getCenterTarget() const;

	/// Accessor method for the dimensions of the food source
	/// @Return: A vector representing the width and height of the food source
	const sf::Vector2f& getDimensions() const;

#pragma endregion

private:
	/// Constants
	const float STANDARD_WIDTH = 200.0f;
	const float STANDARD_HEIGHT = 200.0f;

	/// Fields
	sf::Vector2f mDimensions;
	sf::RectangleShape mBody;
	float mFoodAmount;
};

