#pragma once
#include "Entity.h"
#include <random>
#include <map>
#include <functional>


class Hive;

class Bee : public Entity
{
public:

	enum Type
	{
		Onlooker,
		Employee,
		Drone,
		Guard,
		Queen,
		Larva
	};

	enum State
	{
		Idle,
		Scouting,
		SeekingTarget,
		HarvestingFood,
		DeliveringFood,
		DepositingFood,
	};

	const static float STANDARD_BEE_SPEED;
	const static float BodyRadius;
	const static float TARGET_RADIUS;
	const static float STANDARD_HARVESTING_DURATION;
	const static sf::Color NORMAL_COLOR;
	const static sf::Color ALERT_COLOR;
	const static sf::Color Bee::STANDARD_BODY_COLOR;

#pragma region Construction/Copy/Assignment

	/**
	 * Constructor
	 * @Param position: Starting world position of the bee
	 * @Param hive: The hive that this bee belongs to
	 */
	explicit Bee(const sf::Vector2f& position, Hive& hive);

	virtual ~Bee();

	Bee(const Bee& rhs) = delete;

	Bee& operator=(const Bee& rhs) = delete;

	Bee(Bee&& rhs) = delete;

	Bee& operator=(Bee&& rhs) = delete;

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

	/**
	 * Determines if the bee is colliding with the specified food source
	 * @Param foodSource: The food source being checked
	 * @Return: True if the bee is within the bounds of the food source. False otherwise
	 */
	bool CollidingWithFoodSource(const class FoodSource& foodSource) const;

	/**
	 * Determines if the bee is close enough to detect the specified food source
	 * @Param foodSource; The food source being checked
	 * @return: True if the bee is within the detectable range of the food source. False otherwise
	 */
	bool DetectingFoodSource(const class FoodSource& foodSource) const;

	/**
	 * Determines if the bee is colliding with the specified hive
	 * @Param hive: The hive being checked
	 * @Return: True if the bee is within the bounds of the hive. False otherwise
	 */
	bool CollidingWithHive(const class Hive& hive) const;

	/**
	 * Sets the bee's body color to the specified value
	 * @Param color: The new color of the bee's body
	 */
	void SetColor(const sf::Color& color);

	/**
	 * Sets the bee's target to the specified food source
	 * @Param foodSource: The bee's new food source target
	 */
	void SetTarget(class FoodSource* const foodSource);

	/**
	 * Sets the bee's target tot he specified position
	 * @Param position: The bee's new target position
	 */
	void SetTarget(const sf::Vector2f& position);

	/**
	 * Accessor method for the bee's current target location
	 * @Return: A Vector2f position value of the bee's target
	 */
	const sf::Vector2f& GetTarget() const;

	/**
	 * Determines if the bee currently has an active target
	 * @Return: True if the bee has a target. False otherwise
	 */
	bool HasTarget() const;

	/**
	 * Accessor method for the bee's current food amount
	 * @Return: A float representing how much food the bee currently has
	 */
	float GetFoodAmount() const;

	/**
	 * Adds food to the bee's body
	 * @Param foodAmount: The amount of food being added to the bee
	 */
	void HarvestFood(const float& foodAmount);

	/**
	 * Deposits food from the bee's body to its parent hive
	 * @Param foodAmount; The amount of food being deposited into the hive
	 */
	void DepositFood(float foodAmount);

	/**
	 * Mutator method for the bee's current state
	 * @Param state: The bee's new state
	 */
	void SetState(const State& state);

	/**
	 * Accessor method for the bee's current state
	 * @Return: The state of the bee
	 */
	State GetState() const;

	/**
	 * Determines whether the bee requires food
	 * @Return: True if the bee needs to eat
	 */
	bool Hungry() const;

	/**
	 * Accessor method for the parent hive
	 * @Return: A reference to the hive that this bee belongs to
	 */
	Hive& GetParentHive() const;

protected:

	/**
	 * Method children must do to populate their function maps (if used)
	 * //TODO: Move function map and methods to a component
	 */
	virtual void PopulateFunctionMaps() = 0;

	typedef std::function<void(sf::RenderWindow&, const double&)> UpdateCallback;
	std::map<State, UpdateCallback> mUpdate;
	
	// Constants
	const float PI = 3.14159265359f;
	const float EXTRACTION_YIELD = 5.0f;

	// Private API
	void HandleFoodSourceCollisions();
	void DetectStructureCollisions();

	// Private fields
	Hive& mParentHive;
	std::default_random_engine mGenerator;
	sf::CircleShape mBody;
	sf::RectangleShape mFace;
	sf::Vector2f mTarget;
	sf::Text mText;
	sf::Clock mHarvestingClock;
	float mSpeed;
	float mFoodAmount;
	float mHarvestingDuration;
	float mMaxEnergy;
	float mEnergy;
	float mEnergyConsumptionRate;
	bool mTargeting;
	State mState;
	class FoodSource* mTargetFoodSource;

};
