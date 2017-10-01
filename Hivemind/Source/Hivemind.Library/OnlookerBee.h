#pragma once
#include "Bee.h"
#include <SFML/System/Vector2.hpp>


class OnlookerBee : public Bee
{

public:

#pragma region Construction/Copy/Assignment

	/**
	 * Constructor
	 * @Param position: The posiition of the bee in the world
	 * @Param hive: The hive that the bee belongs to
	 */
	OnlookerBee(const sf::Vector2f& position, Hive& hive);

	virtual ~OnlookerBee() = default;

    OnlookerBee(const OnlookerBee& rhs) = delete;

    OnlookerBee& operator=(const OnlookerBee& rhs) = delete;

    OnlookerBee(OnlookerBee&& rhs) = delete;

    OnlookerBee& operator=(OnlookerBee&& rhs) = delete;

#pragma endregion

	/**
	 * Update method called by the main game loop
	 * @Param window: The window that the simulation is being rendered to
	 * @Param deltaTime: The time since the last Update call
	 */
	void Update(sf::RenderWindow& window, const double& deltaTime) override;

protected:

	void PopulateFunctionMaps() override;

private:

	void UpdateIdle(sf::RenderWindow& window, const double& deltaTime);
	void UpdateSeekingTarget(sf::RenderWindow& window, const double& deltaTime);
	void UpdateHarvestingFood(sf::RenderWindow& window, const double& deltaTime);
	void UpdateDeliveringFood(sf::RenderWindow& window, const double& deltaTime);
	void UpdateDepositingFood(sf::RenderWindow& window, const double& deltaTime);

	/**
	 * Sets the position and rotation of the bee after computed in update
	 * @Param newPosition: The bee's new position after update
	 * @Param rotationAngle: The bee's new rotation after update
	 */
	void PostUpdate(const sf::Vector2f& newPosition, const float& rotationAngle);

};

