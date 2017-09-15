#pragma once
#include "Bee.h"
#include <SFML/System/Vector2.hpp>

/**
*	@Author: Dale Diaz
*	@Date: 7/25/2017
*/

class Larva :public Bee
{
public:

	// TODO: Remove larva type enum and centralize use of Bee::Type
	enum LarvaType
	{
		Drone,
		Employee,
		Onlooker,
		Queen,
		Guard
	};

	/**
	 * Constructor
	 * @Param position: The starting position of the larva
	 * @Param hive: The hive that the larva belongs to
	 * @Param larvaType: What type of bee the larva will spawn as
	 */
	Larva(const sf::Vector2f& position, Hive& hive, const LarvaType& larvaType);

	/**
	 *  Destructor
	 */
	~Larva() = default;

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

	// Larva don't have states so we don't need to use the function map
	virtual void PopulateFunctionMaps() override {}

private:

	/**
	 * Larva destroys itself and spawns
	 */
	void Hatch();

	float mTimeSinceBirth;
	float mLarvaDuration;
	LarvaType mLarvaType;
};

