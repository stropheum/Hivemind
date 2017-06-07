#pragma once
class Bee
{
public:
	const static float STANDARD_BEE_SPEED;
	const static float BODY_RADIUS;
	const static sf::Color NORMAL_COLOR;
	const static sf::Color ALERT_COLOR;

	/// Constructors/destructor
	explicit Bee(const sf::Vector2f& position);
	~Bee() = default;

	/// Copy/Move semantics
	Bee(const Bee& rhs) = default;
	Bee& operator=(const Bee& rhs) = default;
	Bee(Bee&& rhs) = default;
	Bee& operator=(Bee&& rhs) = default;

	/// Public API
	void update(sf::RenderWindow& window, const float& deltaTime);
	void render(sf::RenderWindow& window) const;
	void setPosition(const sf::Vector2f& position);
	const sf::Vector2f& getPosition() const;
	bool collidingWithFoodSource(const class FoodSource& foodSource) const;
	void setColor(const sf::Color& color);
	void setTarget(const sf::Vector2f& position);
	const sf::Vector2f& getTarget() const;
	float distanceBetween(const sf::Vector2f& position_1, const sf::Vector2f& position_2) const;
	bool hasTarget() const;

private:
	/// Constants
	const float PI = 3.14159265359f;

	/// Private fields
	sf::CircleShape mBody;
	sf::RectangleShape mFace;
	sf::Vector2f mPosition;
	sf::Vector2f mTarget;
	float mSpeed;
	bool mTargeting;
};