#pragma once
#include "Bee.h"
#include <SFML/System/Vector2.hpp>


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

#pragma region Construction/Copy/Assignment

	/**
	 * Constructor
	 * @Param position: The starting position of the larva
	 * @Param hive: The hive that the larva belongs to
	 * @Param larvaType: What type of bee the larva will spawn as
	 */
	Larva(const sf::Vector2f& position, Hive& hive, const LarvaType& larvaType);

	~Larva() = default;

    Larva(const Larva& rhs) = delete;

    Larva& operator=(const Larva& rhs) = delete;

    Larva(Larva&& rhs) = delete;

    Larva& operator=(Larva&& rhs) = delete;

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

	// Larva don't have states so we don't need to use the function map
	void PopulateFunctionMaps() override {}

private:

	/**
	 * Larva destroys itself and spawns
	 */
	void Hatch();

	float mTimeSinceBirth;
	float mLarvaDuration;
	LarvaType mLarvaType;

};

