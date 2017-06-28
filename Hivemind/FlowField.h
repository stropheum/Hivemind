#pragma once
#include "Entity.h"
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sf {
	class Color;
	class RenderWindow;
}

class FlowField : public Entity
{
public:
	explicit FlowField(const sf::Vector2f& position);
	~FlowField();

	/// Update method called by the main game loop
	/// @Param window: The window that the simulation is being rendered to
	/// @Param deltaTime: The time since the last update call
	void update(sf::RenderWindow& window, const float& deltaTime) override;

	/// Render method called by the main game loop
	/// @Param window: The window that the simulation is being rendered to
	void render(sf::RenderWindow& window) const override;

	/// Accessor method for field dimensions
	/// @Return: A copy of the integer vector containing the size of the flow field
	sf::Vector2i getDimensions() const;

	/// Determines if a position is within the bounds of the flow field
	/// @Param position: The position in question
	/// @Return: True if the position is within the bounds of the flow field
	bool collidingWith(const sf::Vector2f& position) const;

	/// Regenerates a new random flow field
	void generateNewField();

	void setOctaveCount(const std::uint32_t& octaveCount);

	float radianValueAtPosition(const sf::Vector2f& position) const;

private:
	const sf::Vector2i mFieldDimensions = sf::Vector2i(300, 300);
	sf::Image mImage;
	sf::Sprite mSprite;
	sf::Texture mTexture;
	sf::Uint8** mValues;
	std::uint32_t mOctaveCount;
};

