#pragma once
class Entity
{
public:
	explicit Entity(const sf::Vector2f& position, 
		const sf::Color& outlinecolor = sf::Color::White, 
		const sf::Color& fillColor = sf::Color::White);
	~Entity() = default;

	/// Copy/Move semantics
	Entity(const Entity& rhs) = default;
	Entity& operator=(const Entity& rhs) = default;
	Entity(Entity&& rhs) = default;
	Entity& operator=(Entity&& rhs) = default;

	/// Public API
	static float distanceBetween(const sf::Vector2f& position_1, const sf::Vector2f& position_2);
	void setPosition(const sf::Vector2f& position);
	const sf::Vector2f& getPosition() const;

protected:
	sf::Vector2f mPosition;
	sf::Color mOutlineColor, mFillColor;
};

