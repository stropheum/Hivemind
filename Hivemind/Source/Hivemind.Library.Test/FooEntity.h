#pragma once

/**
*	@Author: Dale Diaz
*	@Date: 7/25/2017
*/

class FooEntity : public Entity
{
public:

	explicit FooEntity(const sf::Vector2f& position,
		const sf::Color& outlineColor = sf::Color::White,
		const sf::Color& fillColor = sf::Color::White);

	virtual ~FooEntity() = default;

	virtual void Update(sf::RenderWindow& window, const double& deltaTime) override;
	virtual void Render(sf::RenderWindow& window) const override;
};

