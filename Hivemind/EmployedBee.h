#pragma once
#include "Bee.h"
#include "Flowfield.h"

namespace sf {
	class RenderWindow;
}

class EmployedBee :
	public Bee
{
public:

	/// Constructor
	/// @Param position: The starting position of the bee
	/// @Param hive: The hive that this bee belongs to
	EmployedBee(const sf::Vector2f& position, class Hive& hive);

	/// Default destructor
	virtual ~EmployedBee() = default;

	/// Update method called by the main game loop
	/// @Param window: The window that the simulation is being rendered to
	/// @Param deltaTime: The time since the last update call
	void update(sf::RenderWindow& window, const float& deltaTime) override;
	
	/// Render method called by the main game loop
	/// @Param window: The window that the simulation is being rendered to
	void render(sf::RenderWindow& window) const override;

	/// Toggles the flow field visualization for the wandering algorithm of the scouting state
	void toggleFlowField();

	/// Sets the number of octaves used for generating the flow field of the scouting state
	/// @Param octaveCount: The number of octaves of noise used for blending of the flow field
	void setFlowFieldOctaveCount(const std::uint32_t& octaveCount);

private:

	/// Performs the waggle dance and disseminates information into the hive
	void waggleDance();

	/// Methods for handling update for specific states
	void updateScouting(sf::RenderWindow& window, const float& deltaTime);
	void updateSeekingTarget(sf::RenderWindow& window, const float& deltaTime);
	void updateHarvestingFood(sf::RenderWindow& window, const float& deltaTime);
	void updateDeliveringFood(sf::RenderWindow& window, const float& deltaTime);
	void updateDepositingFood(sf::RenderWindow& window, const float& deltaTime);
	void updatePosition(const sf::Vector2f& position, const float& rotation);

	/// Updates the flow field based on the wandering state
	/// @Param window: The screen that the game is being rendered to
	/// @Param deltaTime: The time since last update of the flow field
	void updateFlowField(sf::RenderWindow& window, const float& deltaTime);

	class FoodSource* mPairedFoodSource;
	FlowField mFlowField;
	bool mDisplayFlowField;
	sf::VertexArray mLineToFoodSource;
	std::pair<float, float> mFoodSourceData;
};

