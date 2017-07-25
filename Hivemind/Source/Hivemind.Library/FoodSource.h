#pragma once
#include "Entity.h"

/**
*	@Author: Dale Diaz
*	@Date: 7/25/2017
*/

class FoodSource : public Entity
{
public:

	// Constants
	static const float STANDARD_WIDTH;
	static const float STANDARD_HEIGHT;

#pragma region Constructors / Destructor / Move semantics

	/**
	 * Constructor
	 * @Param position: The starting position of the food source
	 */
	explicit FoodSource(const sf::Vector2f& position);

	/**
	 *  Default Destructor
	 */
	virtual ~FoodSource() = default;

	/**
	 * Default Copy Constructor
	 * @Param rhs: The food source being copied
	 */
	FoodSource(const FoodSource& rhs) = delete;

	/**
	 * Default Assignment Operator
	 * @Param rhs: The food source being assigned to
	 * @Return: A copy of the specified food source
	 */
	FoodSource& operator=(const FoodSource& rhs) = delete;

	/**
	 * Default Move Copy Constructor
	 * @Param rhs: The food source being moved
	 */
	FoodSource(FoodSource&& rhs) = delete;

	/**
	 * Default Move Assignment Operator
	 * @Param rhs: The food source being moved
	 * @Return: A reference to the moved food source
	 */
	FoodSource& operator=(FoodSource&& rhs) = delete;

	/**
	 * Takes food from the food source
	 * @Param amount: The amount of food being removed from the source
	 * @Return: The amount of food taken from teh source
	 */
	float TakeFood(const float amount);

	/**
	 * Determines if the food source is viable for harvesting
	 * @Return: True if the food source is viable (current food > 0.0f)
	 */
	bool IsViable() const;

	static float DetectionRadius;

#pragma endregion 

#pragma region Public API

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
	 * Accessor for the amount of food that is currently stored
	 * @Return: The amount of food stored in the source
	 */
	float GetFoodAmount() const;

	/**
	 * Mutator method for the food amount in the source
	 * @Param foodAmount: The new amount of food in the source
	 */
	void SetFoodAmount(const float& foodAmount);

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
	 * Accessor method to determine if the food source is being actively scouted by an employee
	 * @Return: True if the food source is actively being scouted
	 */
	bool PairedWithEmployee() const;

	/**
	 * Mutator method for whether the food source is actively being scouted
	 * @Param pairedWithEmployee: Value determining if the food source is paired or not
	 */
	void SetPairedWithEmployee(const bool& pairedWithEmployee);

#pragma endregion

private:

	// Fields
	sf::Vector2f mDimensions;
	sf::RectangleShape mBody;
	float mFoodAmount;
	sf::Text mText;
	bool mPairedWithEmployee;
};

