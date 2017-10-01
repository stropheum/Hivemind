#pragma once


class CollisionGrid
{
private:

	/**
	 *  Constructor
	 */
	CollisionGrid();

public:

	/**
	 *  Destructor
	 */
	~CollisionGrid();

	/**
	 * Singleton accesssor method
	 * @Return: A pointer to the singleton instance
	 */
	static CollisionGrid* GetInstance();

	/**
	 * Renders the collision grid to the screen, if visible
	 * @Param window: The screen that the grid is being rendered to
	 */
	void Render(sf::RenderWindow& window) const;

	/**
	 *  Turns the visualization of the collision grid on/off
	 */
	void ToggleGridVisualization();

	/**
	 * Gets the collision node responsible for tracking entities in the specified position
	 * @Param position: The world position of the entity in question
	 * @Return: The collision node that is responsible for the position
	 */
	class CollisionNode* CollisionNodeFromPosition(const sf::Vector2f& position) const;

	/**
	 * Gets all valid neighbor nodes of the specified node. Used for handling overlapping collisions
	 * @Param node: The node being checked for neighbors
	 * @Return: A vector containing all valid neighbors of the node
	 */
	std::vector<CollisionNode*> NeighborsOf(CollisionNode* const node) const;

private:
	static CollisionGrid* sInstance;
	class CollisionNode** mGrid;
	int mGridSize;
	int mNodeSize;
	sf::Vector2f mGridOrigin;
	bool mVisible;
};

