#pragma once
#include "Entity.h"
#include "OnlookerBee.h"
#include "HiveHUD.h"


class Hive :
	public Entity
{
public:

	enum BeeType
	{
		Drone,
		Employee,
		Onlooker,
		Queen,
		Guard
	};

	/**
	 * Constructor
	 * @Param position: The starting position of the food source
	 */
	explicit Hive(const sf::Vector2f& position);

	/**
	 *  Destructor
	 */
	virtual ~Hive();

	/**
	 * Updates the current game state of the hive
	 * @Param window: The screen that the game is being rendered to
	 * @Param deltaTime: The time elapsed since last Update
	 */
	void Update(sf::RenderWindow& window, const double& deltaTime) override;

	/**
	 * Renders the hive to the screen
	 * @Param window: The screen which the hive is being rendered to
	 */
	void Render(sf::RenderWindow& window) const override;

	/**
	 * Accessor method for the center point of the food source
	 * @Return: A vector representing the center point of the source
	 */
	sf::Vector2f GetCenterTarget() const;

	/**
	 * Accessor method for the dimensions of the food source
	 * @Return: A vector representing the width and height of the food source
	 */
	const sf::Vector2f& GetDimensions() const;

	/**
	 * Accessor method for the food amount stored in the hive
	 * @Return: A float representing the amount of food in the hive
	 */
	float GetFoodAmount() const;

	/**
	 * Adds food to the hive
	 * @Param foodAmount: Amount of food being added to the hive
	 */
	void DepositFood(const float& foodAmount);

	/**
	 * TakesFood from the hive
	 * @Param foodAmount: Amount of food being taken from the hive
	 * @Return: The amount of food taken
	 */
	float TakeFood(float foodAmount);

	/**
	 * Adds an onlooker to the hive for easy tracking during waggle dance phase
	 * @Param bee: Bee being added as idle to the hive
	 */
	void AddIdleBee(OnlookerBee* const bee);

	/**
	 * Removes an onlooker from the hives collection of idle bees
	 * @Param bee: Bee being removed from the collection of idle bees
	 */
	void RemoveIdleBee(OnlookerBee* const bee);

	/**
	 * Accessor for the Begin iterator of the idle bees
	 * @Return: An iterator pointing to the beginning of the idle bees vector
	 */
	std::vector<OnlookerBee*>::iterator IdleBeesBegin();

	/**
	 * Accessor for the End iterator of the idle bees
	 * @Return: An iterator pointing to the End of the idle bees vector
	 */
	std::vector<OnlookerBee*>::iterator IdleBeesEnd();

	/**
	 *  Iterates over the idle bees vector and removes all bees which are no longer idle
	 */
	void ValidateIdleBees();

	/**
	 *  Deposits food source information into the hive
	 */
	void UpdateKnownFoodSource(class FoodSource* const foodSource, const std::pair<float, float>& foodSourceData);

	/**
	 *  Removes the food source from the list of known food sources, if it exists
	 */
	void RemoveFoodSource(class FoodSource* const foodSource);

	/**
	 *  Causes all bees within the hive to watch the waggle dance and decide on updated food source data
	 */
	void TriggerWaggleDance();

	/**
	 *  After wait period has ended and no new scouts have delivered food, allow bees to choose their food source
	 */
	void CompleteWaggleDance();

	/**
	 * Adds structural comb to the hive
	 * @Param combAmount: The amount of structural comb being added
	 */
	void AddStructuralComb(const float& combAmount);

	/**
	 * Removes structural comb from the hive
	 * @Param combAmount: The amount of structural comb being removed
	 */
	void RemoveStructuralComb(const float& combAmount);

	/**
	 * Converts existing structural comb to honey comb
	 * @Param combAmount: The amount of comb being converted to honey comb
	 */
	void ConvertToHoneyComb(float combAmount);

	/**
	 * Converts existing structural comb to brood comb
	 * @Param combAmount: The amount of comb being converted to brood comb
	 */
	void ConvertToBroodComb(float combAmount);

	/**
	 * Determines if the hive requires additional structural comb
	 * @Return: True if the structural comb is not currently at max. False otherwise
	 */
	bool RequiresStructuralComb() const;

	/**
	 * Determines if the hive needs more comb to store additional honey
	 * @Return: True if the hive needs more honey comb
	 */
	bool RequiresHoneyComb() const;

	/**
	 * Determines if the hive needs more comb to store additional brood
	 * @Return: True if the hive needs more brood comb
	 */
	bool RequiresBroodComb() const;

	/**
	 * Increases the count of bees of a certain type
	 * @Param type: The type of bee being incremented
	 */
	void IncrementBeeCount(const BeeType& type);

	/**
	 * Decreases the count of bees of a certain type
	 * @Param type: The type of bee being decremented
	 */
	void DecrementBeeCount(const BeeType& type);

	/**
	 * Accessor for different bee types in the hive
	 * @Param type: The type of bee being counted
	 * @Return: The number of bees associated with the specified type in the hive
	 */
	int GetBeeCount(const Bee::Type& type) const;

	/**
	 * Determines if a food source is known by the hive
	 * @Param foodSource: The food source under consideration
	 * @Return: True if the food source is currently known by the hive
	 */
	bool FoodSourceIsKnown(FoodSource* const foodSource) const;

private:

	// Constants
	const float STANDARD_WIDTH = 200.0f;
	const float STANDARD_HEIGHT = 200.0f;

	/**
	 * Determines the fitness of the food source based on the minimum and maximum parameters and its relation to them
	 * @Param fooddata: A pair of two floats containing the yield and distance of an individual food source
	 * @Param minYield: The minimum yield of all known food sources
	 * @Param maxYield: The maximum yield of all known food sources
	 * @Param minDistance: The closest distance of all known food sources
	 * @Param maxDistance: The farthest distance of all known food sources
	 * @Return: A float value between 0-1, where 0 is the worst possible fitness and 1 is best possible fitness
	 */
	static float ComputeFitness(const std::pair<float, float>& foodData, const float& minYield, const float& maxYield, const float& minDistance, const float& maxDistance);

	// Fields
	sf::Vector2f mDimensions;
	sf::RectangleShape mBody;
	float mFoodAmount;
	sf::Text mText;
	std::vector<OnlookerBee*> mIdleBees;
	std::map<class FoodSource* const, std::pair<float, float>> mFoodSourceData;
	std::default_random_engine mGenerator;
	sf::Clock mWaggleDanceClock;
	float mWaggleDanceWaitPeriod;
	float mStructuralComb;
	float mHoneyComb;
	float mBroodComb;
	bool mWaggleDanceInProgress;
	int mOnlookerCount, mEmployeeCount, mDroneCount, mGuardCount, mQueenCount;
	HiveHUD mHUD;
};

