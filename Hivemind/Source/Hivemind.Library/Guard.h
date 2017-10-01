#pragma once
#include "Bee.h"
#include <SFML/System/Vector2.hpp>


class Guard :public Bee
{
public:

#pragma region Construction/Copy/Assignment

	/**
	 * Constructor
	 * @Param position: The starting position of the bee
	 * @Param hive: The hive that this bee belongs to
	 */
	Guard(const sf::Vector2f& position, class Hive& hive);

	~Guard() = default;

    Guard(const Guard& rhs) = delete;

    Guard& operator=(const Guard& rhs) = delete;

    Guard(Guard&& rhs) = delete;

    Guard& operator=(Guard&& rhs) = delete;

#pragma endregion

	/**
	 * Update method called by the main game loop
	 * @Param window: The window that the simulation is being rendered to
	 * @Param deltaTime: The time since the last Update call
	 */
	void Update(sf::RenderWindow& window, const double& deltaTime) override;

	/**
	 * Render method called by the main game loop
	 * @Param window: The window that the simulation is being rendered to
	 */
	void Render(sf::RenderWindow& window) const override;

protected:

	// Guards don't use state so we don't need to use the function map
	void PopulateFunctionMaps() override {}
};

