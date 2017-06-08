#pragma once
#include "Entity.h"

class FoodSource : public Entity
{
public:
	/// Constructors/Destructor
	explicit FoodSource(const sf::Vector2f& position);
	~FoodSource() = default;

	/// Copy/Move Semantics
	FoodSource(const FoodSource& rhs) = default;
	FoodSource& operator=(const FoodSource& rhs) = default;
	FoodSource(FoodSource&& rhs) = default;
	FoodSource& operator=(FoodSource&& rhs) = default;

	/// Public API
	void update(sf::RenderWindow& window, const float& deltaTime);
	void render(sf::RenderWindow& window) const;
	float getFoodAmount() const;
	void setFoodAmount(const float& foodAmount);
	sf::Vector2f getCenterTarget() const;
	const sf::Vector2f& getDimensions() const;

private:
	/// Constants
	const float STANDARD_WIDTH = 200.0f;
	const float STANDARD_HEIGHT = 200.0f;

	/// Private API

	/// Fields
	sf::Vector2f mDimensions;
	sf::RectangleShape mBody;
	float mFoodAmount;
};

