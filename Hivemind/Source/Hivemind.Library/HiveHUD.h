#pragma once


class HiveHUD
{
public:

	/**
	 *  Construtor
	 */
	HiveHUD(
		const sf::Vector2f& rootPosition, const sf::Vector2f& dimensions,
		const int& onlookerCount, const int& employeeCount, int& droneCount, const int& guardCount, const int& queenCount,
		const float& structuralComb, const float& honeyComb, const float& broodComb, const float& mFoodAmount);

	/**
	 * Destructor
	 */
	~HiveHUD() = default;

	// Copy / Move semantics
	HiveHUD(const HiveHUD& rhs) = delete;
	HiveHUD& operator=(const HiveHUD& rhs) = delete;
	HiveHUD(HiveHUD&& rhs) = delete;
	HiveHUD& operator=(HiveHUD&& rhs) = delete;

	/**
	 * Renders the HUD to the screen
	 * @Param window: The window that the HUD is being rendered to
	 */
	void Render(sf::RenderWindow& window) const;

	/**
	 *  Updates the relative size representations of the contents of the hive
	 */
	void UpdateHUDValues();

private:

	// Data references
	const int& mOnlookerCount;
	const int& mEmployeeCount;
	const int& mDroneCount;
	const int& mQueenCount;
	const int& mGuardCount;
	const float& mStructuralComb;
	const float& mHoneyComb;
	const float& mBroodComb;
	const float& mFoodAmount;

	// Rendering components
	sf::Vector2f mRootPosition;
	sf::Vector2f mDimensions;
	sf::RectangleShape mBeeContainer;
	sf::RectangleShape mCombContainer;
	sf::RectangleShape mFoodContainer;
	sf::RectangleShape mBarOnlookers;
	sf::RectangleShape mBarEmployees;
	sf::RectangleShape mBarDrones;
	sf::RectangleShape mBarGuards;
	sf::RectangleShape mBarQueens;
	sf::RectangleShape mBarStructuralComb;
	sf::RectangleShape mBarHoneyComb;
	sf::RectangleShape mBarBroodComb;
	sf::RectangleShape mBarFoodAmount;
	
	float mOutlineThickness;
};

