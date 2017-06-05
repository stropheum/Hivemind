#pragma once
class Bee
{
public:
	Bee();
	explicit Bee(const sf::Vector2f& position);
	~Bee() = default;
	void update(sf::RenderWindow& window, const float& deltaTime);
	void render(sf::RenderWindow& window) const;
	void setPosition(const sf::Vector2f& position);
	const sf::Vector2f& getPosition() const;
	float getRadius() const;

private:
	const float mBodyRadius = 7.0f;
	const float pi = 3.14159265359f;

	float distanceBetween(const sf::Vector2f& position_1, const sf::Vector2f& position_2) const;
	
	sf::CircleShape mBody;
	sf::RectangleShape mFace;
	sf::Vector2f mPosition;
	float speed;
};

