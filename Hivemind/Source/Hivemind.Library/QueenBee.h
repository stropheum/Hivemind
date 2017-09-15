#pragma once
#include "Bee.h"
#include <SFML/System/Vector2.hpp>

/**
*	@Author: Dale Diaz
*	@Date: 7/25/2017
*/

class QueenBee : public Bee
{
public:

	/**
	 * Constructor
	 * @Param position: The starting position of the bee
	 * @Param hive: The hive that this bee belongs to
	 */
	explicit QueenBee(const sf::Vector2f& position, Hive& hive);

	/**
	 * Destructor
	 */
	~QueenBee() = default;

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
	
	// Bees don't operate on state so we don't need to use the function map
	virtual void PopulateFunctionMaps() override {}

private:

	float mLarvaDepositInterval;
	float mTimeSinceLarvaDeposit;

};

