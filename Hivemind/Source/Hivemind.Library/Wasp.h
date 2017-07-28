#pragma once
#include "Entity.h"

/**
* @Author: Dale Diaz
* @Date: 7/26/2017
*/

class Wasp :
	public Entity
{

public:

	const static float StandardWaspSpeed;

	enum State
	{
		Wandering,
		Attacking
	};

	/**
	 * Constructor
	 * @Param position: The starting position of the wasp
	 */
	explicit Wasp(const sf::Vector2f& position);

	/**
	 * Destructor
	 */
	~Wasp();

	/**
	 * Updates the wasp's current state
	 * @Param window: The screen which the wasp is being rendered to
	 * @Param deltaTime: The time since last update call on the wasp
	 */
	void Update(sf::RenderWindow& window, const double& deltaTime) override;

	/**
	 * Renders the wasp to the specified screen
	 * @Param window: The screen which the wasp is being rendered to
	 */
	void Render(sf::RenderWindow& window) const override;

	/**
	 * Accessor for the hive that the wasp is currently attacking, if any
	 * @Return: A pointer to the hive being attacked, if any. Nullptr otherwise
	 */
	Hive* GetTargetHive() const;

private:

	/**
	 * Generates a new random target for the wasp to pursue
	 */
	void GenerateNewTarget();

	/**
	* Updates the wasp's current state while wandering
	* @Param window: The screen which the wasp is being rendered to
	* @Param deltaTime: The time since last update call on the wasp
	*/
	void UpdateWanderingState(sf::RenderWindow& window, const double& deltaTime);

	/**
	* Updates the wasp's current state while attacking
	* @Param window: The screen which the wasp is being rendered to
	* @Param deltaTime: The time since last update call on the wasp
	*/
	void UpdateAttackingState(sf::RenderWindow& window, const double& deltaTime);

	sf::CircleShape mBody;
	State mState;
	sf::Vector2f mTarget;
	std::default_random_engine mGenerator;
	Hive* mTargetHive;
};

