#pragma once
#include "Entity.h"


class Wasp;

class CollisionNode :public Entity
{

public:

	/**
	 *  Default Constructor
	 */
	CollisionNode();

	/**
	 *  Constructor
	 */
	CollisionNode(const sf::Vector2f& position, const int& size);

	/**
	 *  Destructor
	 */
	virtual ~CollisionNode();

	/**
	 * Updates the Collision Node
	 * @Param window: The window that the collision node is being rendered to
	 * @Param deltaTime: The time since last update call
	 */
	void Update(sf::RenderWindow& window, const double& deltaTime) override;

	/**
	 * Renders the collision node to the window
	 * @Param window: The window that the collision node is being rendered to
	 */
	void Render(sf::RenderWindow& window) const override;

	/**
	 * Sets the position of the entity
	 * @Param entity: the new position of the entity
	 */
	void SetPosition(const sf::Vector2f& position) override;

	/**
	 * Mutator method for size
	 * @Param size: The new size of the collision node
	 */
	void SetSize(const int& size);

	/**
	 * Accessor method for size
	 * @Return: An integer representing the square dimensios of the collision node
	 */
	int GetSize() const;

	/**
	 * Adds a hive to the known entities of the collision node
	 * @Param hive: The hive being added to the collision node
	 */
	void RegisterHive(Hive* const hive);

	/**
	* Removes a hive from the known entities of the collision node
	* @Param hive: The hive being removed from the collision node
	*/
	void UnregisterHive(Hive* const hive);

	/**
	* Adds a food source to the known entities of the collision node
	* @Param foodSource: The food source being added to the collision node
	*/
	void RegisterFoodSource(FoodSource* const foodSource);

	/**
	* Removes a food source to the known entities of the collision node
	* @Param foodSource: The food source being removed from the collision node
	*/
	void UnregisterFoodSource(FoodSource* const foodSource);

	/**
	* Adds a bee to the known entities of the collision node
	* @Param bee: The bee being added to the collision node
	*/
	void RegisterBee(Bee* const bee);

	/**
	* Removes a bee from the known entities of the collision node
	* @Param bee: The bee being removed from the collision node
	*/
	void UnregisterBee(Bee* const bee);

	/**
	 * Adds a wasp to the known entities of the collision node
	 * @Param wasp: The wasp being added to the collision node
	 */
	void RegisterWasp(Wasp* const wasp);

	/**
	 * Removes a wasp from the known entities of the collision node
	 * @Param wasp: The wasp being removed from the collision node
	 */
	void UnregisterWasp(Wasp* const wasp);

	/**
	 * Determines if the specified point is contained within the collision node
	 * @Param point: The point being checked
	 * @Return: True if the point is colliding with the collision node. False otherwise
	 */
	bool ContainsPoint(const sf::Vector2f& point) const;

	/**
	 * Accessor method for the list of hive pointers registered with the collision node
	 * @Return: A vector of hive pointers currently registered with the collision node
	 */
	std::vector<Hive*> Hives() const;

	/**
	* Accessor method for the list of food source pointers registered with the collision node
	* @Return: A vector of food source pointers currently registered with the collision node
	*/
	std::vector<FoodSource*> FoodSources() const;

	/**
	* Accessor method for the list of bee pointers registered with the collision node
	* @Return: A vector of bee pointers currently registered with the collision node
	*/
	std::vector<Bee*> Bees() const;

	/**
	 * Accessor method for the list of wasp pointers registered with the collision node
	 * @Return: A vector of wasp pointers currently registered with the collision node;
	 */
	std::vector<Wasp*> Wasps() const;

private:

	/**
	 * Updates the textual representation of the data contained in the collision node
	 */
	void UpdateTextDisplay();

	/**
	 *  Border for debug rendering
	 */
	sf::RectangleShape mBorder;

	/**
	 *  Text for displaying debug info
	 */
	sf::Text mText;

	/**
	 *  Lists of registered entities in the collision node
	 */
	std::vector<Hive*> mHives;
	std::vector<FoodSource*> mFoodSources;
	std::vector<Bee*> mBees;
	std::vector<Wasp*> mWasps;

	// The square size of the collision node
	int mSize;
};

