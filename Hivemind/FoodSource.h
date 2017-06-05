#pragma once
class FoodSource
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

private:
	/// Constants
	const float STANDARD_WIDTH = 100.0f;
	const float STANDARD_HEIGHT = 100.0f;

	/// Private API

	/// Fields
	float mWidth, mHeight;
	sf::RectangleShape mBody;
	float mFoodAmount;
};

