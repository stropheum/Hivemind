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
	virtual ~FoodSource() = default;

	/// Default Copy Constructor
	/// @Param rhs: The food source being copied
	FoodSource(const FoodSource& rhs) = delete;

	/// Default Assignment Operator
	/// @Param rhs: The food source being assigned to
	/// @Return: A copy of the specified food source
	FoodSource& operator=(const FoodSource& rhs) = delete;

	/// Default Move Copy Constructor
	/// @Param rhs: The food source being moved
	FoodSource(FoodSource&& rhs) = delete;

	/// Default Move Assignment Operator
	/// @Param rhs: The food source being moved
	/// @Return: A reference to the moved food source
	FoodSource& operator=(FoodSource&& rhs) = delete;

	float takeFood(const float amount);

#pragma endregion 

#pragma region Public API

	void update(sf::RenderWindow& window, const float& deltaTime) override;

	void render(sf::RenderWindow& window) const override;

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

	/// Accessor method to determine if the food source is being actively scouted by an employee
	/// @Return: True if the food source is actively being scouted
	bool getPairedWithEmployee() const;

	/// Mutator method for whether the food source is actively being scouted
	/// @Param pairedWithEmployee: Value determining if the food source is paired or not
	void setPairedWithEmployee(const bool& pairedWithEmployee);

#pragma endregion

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
	bool mPairedWithEmployee;
};

