#pragma once
#include "Entity.h"
class Hive : public Entity
{
public:
	/// Constructor
	/// @Param position: The starting position of the food source
	explicit Hive(const sf::Vector2f& position);

	/// Default Destructor
	~Hive() = default;

	/// Default Copy Constructor
	/// @Param rhs: The food source being copied
	Hive(const Hive& rhs) = default;

	/// Default Assignment Operator
	/// @Param rhs: The food source being assigned to
	/// @Return: A copy of the specified food source
	Hive& operator=(const Hive& rhs) = default;

	/// Default Move Copy Constructor
	/// @Param rhs: The food source being moved
	Hive(Hive&& rhs) = default;

	/// Default Move Assignment Operator
	/// @Param rhs: The food source being moved
	/// @Return: A reference to the moved food source
	Hive& operator=(Hive&& rhs) = default;

	void update(sf::RenderWindow& window, const float& deltaTime) override;

	void render(sf::RenderWindow& window) const override;

private:
	/// Constants
	const float STANDARD_WIDTH = 200.0f;
	const float STANDARD_HEIGHT = 200.0f;

	float mFoodAmount;
	sf::Vector2f mDimensions;
	sf::RectangleShape mBody;
	sf::Text mText;
	sf::Font mFont;
};

