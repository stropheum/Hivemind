#pragma once
#include "Entity.h"
#include <random>
#include <chrono>


class Hive;

class Bee : public Entity
{
public:

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
	const static float BODY_RADIUS;
	const static float TARGET_RADIUS;
	const static float STANDARD_HARVESTING_DURATION;
	const static sf::Color NORMAL_COLOR;
	const static sf::Color ALERT_COLOR;
	const static sf::Color Bee::STANDARD_BODY_COLOR;

#pragma region Constructors/Destructor

	/// Constructor
	/// @Param position: Starting p osition of the bee
	/// @Param hive: The hive that this bee belongs to
	explicit Bee(const sf::Vector2f& position, Hive& hive);

	/// Default destructor
	virtual ~Bee() = default;

#pragma endregion

#pragma region Copy/Move Semantics

	/// Copy constructor
	/// @Param rhs: Bee being copied
	Bee(const Bee& rhs) = delete;

	/// Assignment operator
	/// @Param rhs: Bee being copied
	/// @Return: A reference to the copied bee
	Bee& operator=(const Bee& rhs) = delete;

	/// Move copy constructor
	/// @Param rhs: The bee being moved
	Bee(Bee&& rhs) = delete;

	/// Move assignment operator
	/// @Param rhs: The bee being moved
	/// @Return: A reference to the moved bee
	Bee& operator=(Bee&& rhs) = delete;

#pragma endregion

#pragma region Public API

	/// Update method called by the main game loop
	/// @Param window: The window that the simulation is being rendered to
	/// @Param deltaTime: The time since the last update call
	void update(sf::RenderWindow& window, const float& deltaTime) override = 0;

	/// Render method called by the main game loop
	/// @Param window: The window that the simulation is being rendered to
	void render(sf::RenderWindow& window) const override;

	/// Determines if the bee is colliding with the specified food source
	/// @Param foodSource: The food source being checked
	/// @Return: True if the bee is within the bounds of the food source. False otherwise
	bool collidingWithFoodSource(const class FoodSource& foodSource) const;

	/// Determines if the bee is colliding with the specified hive
	/// @Param hive: The hive being checked
	/// @Return: True if the bee is within the bounds of the hive. False otherwise
	bool collidingWithHive(const class Hive& hive) const;

	/// Sets the bee's body color to the specified value
	/// @Param color: The new color of the bee's body
	void setColor(const sf::Color& color);

	/// Sets the bee's target to the specified food source
	/// @Param foodSource: The bee's new food source target
	void setTarget(class FoodSource* const foodSource);

	/// Sets the bee's target tot he specified position
	/// @Param position: The bee's new target position
	void setTarget(const sf::Vector2f& position);

	/// Accessor method for the bee's current target location
	/// @Return: A Vector2f position value of the bee's target
	const sf::Vector2f& getTarget() const;

	/// Determines if the bee currently has an active target
	/// @Return: True if the bee has a target. False otherwise
	bool hasTarget() const;

	/// Deposits food from the bee's body to its parent hive
	/// @Param foodAmount; The amount of food being deposited into the hive
	void depositFood(float foodAmount);

	/// Mutator method for the bee's current state
	/// @Param state: The bee's new state
	void setState(const State& state);

	/// Accessor method for the bee's current state
	/// @Return: The state of the bee
	State getState() const;

#pragma endregion

protected:
	/// Constants
	const float PI = 3.14159265359f;
	const float EXTRACTION_YIELD = 10.0f;

	/// Private API
	void handleFoodSourceCollisions();
	void detectStructureCollisions();

	/// Private fields
	Hive& mParentHive;
	std::default_random_engine mGenerator;
	sf::CircleShape mBody;
	sf::RectangleShape mFace;
	sf::Vector2f mTarget;
	sf::Text mText;
	sf::Font mFont;
	sf::Clock mHarvestingClock;
	float mSpeed;
	float mFoodAmount;
	float mHarvestingDuration;
	bool mTargeting;
	State mState;
	class FoodSource* mTargetFoodSource;
};
