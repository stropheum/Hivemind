#pragma once
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>


class FlowField : public Entity
{
public:

	/// Constructor
	/// @param position: The positio of the flow field
	explicit FlowField(const sf::Vector2f& position);

	/// Destructor
	~FlowField();

	/// Update method called by the main game loop
	/// @Param window: The window that the simulation is being rendered to
	/// @Param deltaTime: The time since the last Update call
	void Update(sf::RenderWindow& window, const float& deltaTime) override;

	/// Render method called by the main game loop
	/// @Param window: The window that the simulation is being rendered to
	void Render(sf::RenderWindow& window) const override;

	/// Accessor method for field dimensions
	/// @Return: A copy of the integer vector containing the size of the flow field
	sf::Vector2i GetDimensions() const;

	/// Determines if a position is within the bounds of the flow field
	/// @Param position: The position in question
	/// @Return: True if the position is within the bounds of the flow field
	bool CollidingWith(const sf::Vector2f& position) const;

	/// Regenerates a new random flow field
	void GenerateNewField();

	/// Mutator method for the number of octaves being calculated for the flow field
	/// @Param octaveCount: The number of octaves in the flow field
	void SetOctaveCount(const std::uint32_t& octaveCount);

	/// Converts the percentace value of the flow field to a radian value (0-1 >> 0-2 mapping)
	/// @Param position: World position vector. Will be converted to relative flow field position
	/// @Return: A float value from 0-2 which will correspond to a radian rotation value
	float RadianValueAtPosition(const sf::Vector2f& position) const;

private:
	const sf::Vector2i mFieldDimensions = sf::Vector2i(300, 300);
	sf::Image mImage;
	sf::Sprite mSprite;
	sf::Texture mTexture;
	sf::Uint8** mValues;
	std::uint32_t mOctaveCount;
};

