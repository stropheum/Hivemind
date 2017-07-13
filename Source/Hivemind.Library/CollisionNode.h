#pragma once
#include "Entity.h"
class CollisionNode :public Entity
{
public:

	/// Default Constructor
	CollisionNode();

	/// Constructor
	CollisionNode(const sf::Vector2f& position, const int& size);

	/// Destructor
	~CollisionNode();

	/// Updates the Collision Node
	/// @Param window: The window that the collision node is being rendered to
	/// @Param deltaTime: The time since last update call
	virtual void Update(sf::RenderWindow& window, const float& deltaTime) override;

	/// Renders the collision node to the window
	/// @Param window: The window that the collision node is being rendered to
	void Render(sf::RenderWindow& window) const override;

	/// Sets the position of the entity
	/// @Param entity: the new position of the entity
	void SetPosition(const sf::Vector2f& position) override;

	/// Mutator method for size
	/// @Param size: The new size of the collision node
	void SetSize(const int& size);

	/// Accessor method for size
	/// @Return: An integer representing the square dimensios of the collision node
	int GetSize() const;

	void RegisterHive(Hive* const hive);

	void UnregisterHive(Hive* const hive);

	void RegisterFoodSource(FoodSource* const foodSource);

	void UnregisterFoodSource(FoodSource* const foodSource);

	void RegisterBee(Bee* const bee);

	void UnregisterBee(Bee* const bee);

	bool ContainsPoint(const sf::Vector2f& point) const;

private:

	void UpdateTextDisplay();

	/// Border for debug rendering
	sf::RectangleShape mBorder;

	/// Text for displaying debug info
	sf::Text mText;

	/// Lists of registered entities in the collision node
	std::vector<Hive*> mHives;
	std::vector<FoodSource*> mFoodSources;
	std::vector<Bee*> mBees;

	/// The square size of the collision node
	int mSize;
};

