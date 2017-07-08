#pragma once
class FooEntity : public Entity
{
public:

	explicit FooEntity(const sf::Vector2f& position,
		const sf::Color& outlineColor = sf::Color::White,
		const sf::Color& fillColor = sf::Color::White);

	virtual ~FooEntity() = default;

	virtual void Update(sf::RenderWindow& window, const float& deltaTime) override;
	virtual void Render(sf::RenderWindow& window) const override;
};

