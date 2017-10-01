#pragma once
#include "Bee.h"
#include <SFML/System/Vector2.hpp>


class QueenBee : public Bee
{

public:
    
#pragma region Construction/Copy/Assignment

	/**
	 * Constructor
	 * @Param position: The starting position of the bee
	 * @Param hive: The hive that this bee belongs to
	 */
	explicit QueenBee(const sf::Vector2f& position, Hive& hive);

	~QueenBee() = default;

    QueenBee(const QueenBee& rhs) = delete;

    QueenBee& operator=(const QueenBee& rhs) = delete;

    QueenBee(QueenBee&& rhs) = delete;

    QueenBee& operator=(QueenBee&& rhs) = delete;

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
	
	// Bees don't operate on state so we don't need to use the function map
	void PopulateFunctionMaps() override {}

private:

	float mLarvaDepositInterval;
	float mTimeSinceLarvaDeposit;

};

