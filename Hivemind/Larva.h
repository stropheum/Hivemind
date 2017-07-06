#pragma once
#include "Bee.h"
#include <SFML/System/Vector2.hpp>


class Larva :public Bee
{
public:

	enum LarvaType
	{
		Drone,
		Employee,
		Onlooker,
		Queen,
		Guard
	};

	/// Constructor
	/// @Param position: The starting position of the larva
	/// @Param hive: The hive that the larva belongs to
	/// @Param larvaType: What type of bee the larva will spawn as
	Larva(const sf::Vector2f& position, Hive& hive, const LarvaType& larvaType);

	/// Destructor
	~Larva() = default;

	/// Update method called by the main game loop
	/// @Param window: The window that the simulation is being rendered to
	/// @Param deltaTime: The time since the last Update call
	void Update(sf::RenderWindow& window, const float& deltaTime) override;

	/// Render method called by the main game loop
	/// @Param window: The window that the simulation is being rendered to
	void Render(sf::RenderWindow& window) const override;

	/// Determines if the larva is marked for delete
	/// @Return: True if the larva is marked for delete. false otherwise
	bool MarkedForDelete() const;

private:

	/// Larva destroys itself and spawns
	void Hatch();

	float mTimeSinceBirth;
	float mLarvaDuration;
	LarvaType mLarvaType;
	bool mMarkedForDelete;
};

