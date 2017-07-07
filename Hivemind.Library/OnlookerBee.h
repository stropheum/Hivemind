#pragma once
#include "Bee.h"
#include <SFML/System/Vector2.hpp>

class OnlookerBee :
	public Bee
{
public:

	/// Constructor
	/// @Param position: The posiition of the bee in the world
	/// @Param hive: The hive that the bee belongs to
	OnlookerBee(const sf::Vector2f& position, Hive& hive);

	/// Destructor
	virtual ~OnlookerBee() = default;

	/// Update method called by the main game loop
	/// @Param window: The window that the simulation is being rendered to
	/// @Param deltaTime: The time since the last Update call
	void Update(sf::RenderWindow& window, const float& deltaTime) override;
};

