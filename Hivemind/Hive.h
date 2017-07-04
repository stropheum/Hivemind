#pragma once
#include "Entity.h"
#include "OnlookerBee.h"

class Hive :
	public Entity
{
public:
	/// Constructor
	/// @Param position: The starting position of the food source
	explicit Hive(const sf::Vector2f& position);
	
	/// Destructor
	virtual ~Hive();

	/// Updates the current game state of the hive
	/// @Param window: The screen that the game is being rendered to
	/// @Param deltaTime: The time elapsed since last Update
	void Update(sf::RenderWindow& window, const float& deltaTime) override;
	
	/// Renders the hive to the screen
	/// @Param window: The screen which the hive is being rendered to
	void Render(sf::RenderWindow& window) const override;

	/// Accessor method for the center point of the food source
	/// @Return: A vector representing the center point of the source
	sf::Vector2f GetCenterTarget() const;

	/// Accessor method for the dimensions of the food source
	/// @Return: A vector representing the width and height of the food source
	const sf::Vector2f& GetDimensions() const;
	
	/// Adds food to the hive
	/// @Param foodAmount: Amount of food being added to the hive
	void DepositFood(const float& foodAmount);

	/// Adds an onlooker to the hive for easy tracking during waggle dance phase
	/// @Param bee: Bee being added as idle to the hive
	void AddIdleBee(OnlookerBee* const bee);

	/// Removes an onlooker from the hives collection of idle bees
	/// @Param bee: Bee being removed from the collection of idle bees
	void RemoveIdleBee(OnlookerBee* const bee);

	/// Accessor for the Begin iterator of the idle bees
	/// @Return: An iterator pointing to the beginning of the idle bees vector
	std::vector<OnlookerBee*>::iterator IdleBeesBegin();

	/// Accessor for the End iterator of the idle bees
	/// @Return: An iterator pointing to the End of the idle bees vector
	std::vector<OnlookerBee*>::iterator IdleBeesEnd();

	/// Iterates over the idle bees vector and removes all bees which are no longer idle
	void ValidateIdleBees();

	/// Deposits food source information into the hive
	void UpdateKnownFoodSource(class FoodSource* const foodSource, const std::pair<float, float>& foodSourceData);

	/// Causes all bees within the hive to watch the waggle dance and decide on updated food source data
	void TriggerWaggleDance();

	/// After wait period has ended and no new scouts have delivered food, allow bees to choose their food source
	void CompleteWaggleDance();

private:
	/// Constants
	const float STANDARD_WIDTH = 200.0f;
	const float STANDARD_HEIGHT = 200.0f;

	/// Determines the fitness of the food source based on the minimum and maximum parameters and its relation to them
	/// @Param fooddata: A pair of two floats containing the yield and distance of an individual food source
	/// @Param minYield: The minimum yield of all known food sources
	/// @Param maxYield: The maximum yield of all known food sources
	/// @Param minDistance: The closest distance of all known food sources
	/// @Param maxDistance: The farthest distance of all known food sources
	/// @Return: A float value between 0-1, where 0 is the worst possible fitness and 1 is best possible fitness
	static float ComputeFitness(const std::pair<float, float>& foodData, const float& minYield, const float& maxYield, const float& minDistance, const float& maxDistance);

	/// Fields
	sf::Vector2f mDimensions;
	sf::RectangleShape mBody;
	float mFoodAmount;
	sf::Font mFont;
	sf::Text mText;
	std::vector<OnlookerBee*> mIdleBees;
	std::map<class FoodSource* const, std::pair<float, float>> mFoodSourceData;
	std::default_random_engine mGenerator;
	sf::Clock mWaggleDanceClock;
	float mWaggleDanceWaitPeriod;
	bool mWaggleDanceInProgress;
};

