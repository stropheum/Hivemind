#pragma once
#include "Bee.h"
#include "Flowfield.h"

/**
*	@Author: Dale Diaz
*	@Date: 7/25/2017
*/

namespace sf {
	class RenderWindow;
}

class EmployedBee :
	public Bee
{
public:

	/**
	 * Constructor
	 * @Param position: The starting position of the bee
	 * @Param hive: The hive that this bee belongs to
	 */
	EmployedBee(const sf::Vector2f& position, class Hive& hive);

	/**
	 *  Default destructor
	 */
	virtual ~EmployedBee() = default;

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

	/**
	 *  Toggles the flow field visualization for the wandering algorithm of the scouting state
	 */
	void ToggleFlowField();

	/**
	 * Sets the number of octaves used for generating the flow field of the scouting state
	 * @Param octaveCount: The number of octaves of noise used for blending of the flow field
	 */
	void SetFlowFieldOctaveCount(const std::uint32_t& octaveCount);

private:

	/**
	 *  Performs the waggle dance and disseminates information into the hive
	 */
	void WaggleDance() const;

	/**
	 * Updates the bee during its scouting state
	 * @Param window: The window that the simulation is being rendered to
	 * @Param deltaTime: The time since the last Update call
	 */
	void UpdateScouting(sf::RenderWindow& window, const float& deltaTime);

	/**
	 * Updates the bee during its seeking target state
	 * @Param window: The window that the simulation is being rendered to
	 * @Param deltaTime: The time since the last Update call
	 */
	void UpdateSeekingTarget(sf::RenderWindow& window, const float& deltaTime);

	/**
	 * Updates the bee during its harvesting food state
	 * @Param window: The window that the simulation is being rendered to
	 * @Param deltaTime: The time since the last Update call
	 */
	void UpdateHarvestingFood(sf::RenderWindow& window, const float& deltaTime);

	/**
	 * Updates the bee during its delivering food state
	 * @Param window: The window that the simulation is being rendered to
	 * @Param deltaTime: The time since the last Update call
	 */
	void UpdateDeliveringFood(sf::RenderWindow& window, const float& deltaTime);

	/**
	 * Updates the bee during its depositing food state
	 * @Param window: The window that the simulation is being rendered to
	 * @Param deltaTime: The time since the last Update call
	 */
	void UpdateDepositingFood(sf::RenderWindow& window, const float& deltaTime);

	/**
	 * Updates the bees position
	 * @Param window: The window that the simulation is being rendered to
	 * @Param deltaTime: The time since the last Update call
	 */
	void UpdatePosition(const sf::Vector2f& position, const float& rotation);

	/**
	 * Updates the flow field based on the wandering state
	 * @Param window: The screen that the game is being rendered to
	 * @Param deltaTime: The time since last Update of the flow field
	 */
	void UpdateFlowField(sf::RenderWindow& window, const float& deltaTime);

	class FoodSource* mPairedFoodSource;
	FlowField mFlowField;
	bool mDisplayFlowField;
	sf::VertexArray mLineToFoodSource;
	std::pair<float, float> mFoodSourceData;
	bool mAbandoningFoodSource;
	sf::Vector2f mVelocity;
};

