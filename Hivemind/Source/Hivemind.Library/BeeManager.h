#pragma once
#include "Bee.h"
#include <random>
#include "Larva.h"

/**
*	@Author: Dale Diaz
*	@Date: 7/21/2017
*/

class BeeManager
{
public:

	/**
	 * Accessor method for the singleton
	 * @return: The singleton instance
	 */
	static BeeManager* GetInstance();

	/**
	 *  Destructor
	 */
	~BeeManager();

	/**
	 * Spawns an Onlooker bee and adds it to the local vector of onlooker bees
	 * @Param position: The position of the bee being spawned
	 * @Param hive: The hive that the bee belongs to
	 */
	void SpawnOnlooker(const sf::Vector2f& position, Hive& hive);

	/**
	 * Spawns an Employed bee and adds it to the local vector of employed bees
	 * @Param position: The position of the bee being spawned
	 * @Param hive: The hive that the bee belongs to
	 */
	void SpawnEmployee(const sf::Vector2f& position, Hive& hive);

	/**
	 * Spawns a Queen bee and adds it to the local vector of queen bees
	 * @Param position: The position of the bee being spawned
	 * @Param hive: The hive that the bee belongs to
	 */
	void SpawnQueen(const sf::Vector2f& position, Hive& hive);

	/**
	 * Spawns a Drone bee and adds it to the local vector of drone bees
	 * @Param position: The position of the bee being spawned
	 * @Param hive: The hive that the bee belongs to
	 */
	void SpawnDrone(const sf::Vector2f& position, Hive& hive);

	/**
	 * Spawns a Guard bee and adds it to the local vector of guard bees
	 * @Param position: The position of the bee being spawned
	 * @Param hive: The hive that the bee belongs to
	 */
	void SpawnGuard(const sf::Vector2f& position, Hive& hive);

	/**
	 * Spawns a Larva and adds it to the local vector of larva
	 * @Param position: The position of the larva being spawned
	 * @Param hive: The hive that the larva belongs to
	 */
	void SpawnLarva(const sf::Vector2f& position, Hive& hive, const Larva::LarvaType& larvaType);

	/**
	 * Disseminates update calls to all bees in the simulation
	 * @Param window: The window that the bees are being displayed to
	 * @Param DeltaTime: The time since last update was called
	 */
	void Update(sf::RenderWindow& window, const float& deltaTime);

	/**
	 * Disseminates render callst o all the bees in the simulation
	 * @Param window: The window tha tthe bees are being displayed to
	 */
	void Render(sf::RenderWindow& window);

	/**
	 * Accessor method for the begin iterator of the onlooker bees collection
	 * @Return: An iterator pointing to the beginning of the list of onlooker bees
	 */
	std::vector<class OnlookerBee*>::iterator OnlookerBegin();

	/**
	 * Accessor method for the end iterator of the onlooker bees collection
	 * @Return: An iterator pointing to the end of the list of onlooker bees
	 */
	std::vector<class OnlookerBee*>::iterator OnlookerEnd();

	/**
	 * Accessor method for the begin iterator of the employed bees collection
	 * @Return: An iterator pointing to the beginning of the list of employed bees
	 */
	std::vector<class EmployedBee*>::iterator EmployeeBegin();

	/**
	 * Accessor method for the end iterator of the employed bees collection
	 * @Return: An iterator pointing to the end of the list of employed bees
	 */
	std::vector<class EmployedBee*>::iterator EmployeeEnd();

	/**
	 * Accessor method for the begin iterator of the queen bees collection
	 * @Return: An iterator pointing to the beginning of the list of queen bees
	 */
	std::vector<class QueenBee*>::iterator QueenBegin();

	/**
	 * Accessor method for the end iterator of the queen bees collection
	 * @Return: An iterator pointing to the end of the list of queen bees
	 */
	std::vector<class QueenBee*>::iterator QueenEnd();

	/**
	 * Accessor method for the begin iterator of the drone bees collection
	 * @Return: An iterator pointing to the beginning of the list of guardbees
	 */
	std::vector<class Drone*>::iterator DroneBegin();

	/**
	 * Accessor method for the end iterator of the drone bees collection
	 * @Return: An iterator pointing to the end of the list of drone bees
	 */
	std::vector<class Drone*>::iterator DroneEnd();

	/**
	 * Accessor method for the begin iterator of the guard bees collection
	 * @Return: An iterator pointing to the beginning of the list of guard bees
	 */
	std::vector<class Guard*>::iterator GuardBegin();

	/**
	 * Accessor method for the end iterator of the guard bees collection
	 * @Return: An iterator pointing to the end of the list of guard bees
	 */
	std::vector<class Guard*>::iterator GuardEnd();

	/**
	 * Accessor method for the begin iterator of the larva collection
	 * @Return: An iterator pointing to the beginning of the list of larva
	 */
	std::vector<class Larva*>::iterator LarvaBegin();

	/**
	 * Accessor method for the end iterator of the larva collection
	 * @Return: An iterator pointing to the end of the list of larva
	 */
	std::vector<class Larva*>::iterator LarvaEnd();

	/**
	 * Accessor method for the size of the onlooker bee list
	 * @Return: The total number of living onlooker bees in the simulation
	 */
	std::uint32_t OnlookerCount() const;

	/**
	 * Accessor method for the size of the employed bee list
	 * @Return: The total number of living employed bees in the simulation
	 */
	std::uint32_t EmployeeCount() const;

	/**
	 * Accessor method for the size of the queen bee list
	 * @Return: The total number of living queen bees in the simulation
	 */
	std::uint32_t QueenCount() const;

	/**
	 * Accessor method for the size of the drone bee list
	 * @Return: The total number of living drone bees in the simulation
	 */
	std::uint32_t DroneCount() const;

	/**
	 * Accessor method for the size of the guard bee list
	 * @Return: The total number of living drone bees in the simulation
	 */
	std::uint32_t GuardCount() const;

	/**
	 * Accessor method for the size of the larva list
	 * @Return: The total number of living larva in the simulation
	 */
	std::uint32_t LarvaCount() const;

	/**
	 *  Toggles the flow field visualization for all employed bees
	 */
	void ToggleEmployeeFlowFields();

	/**
	 * Method to change the number of octaves that are generated for the perlin noise flow fields
	 * @Param octaveCount: The number of octaves that will be used to generate the flow fields
	 */
	void SetEmployeeFlowFieldOctaveCount(const std::uint32_t& octaveCount);

private:
	
	/**
	 *  Constructor
	 */
	BeeManager();

	/**
	 * Removes all bees marked for delete
	 */
	void CleanupBees();

	/**
	 * Removes all onlookers marked for delete
	 */
	void CleanupOnlookers();

	/**
	 * Removes all employees marked for delete
	 */
	void CleanupEmployees();

	/**
	 * Removes all queens marked for delete
	 */
	void CleanupQueens();

	/**
	 * Removes all droens marked for delete
	 */
	void CleanupDrones();

	/**
	 * Removes all guards marked for delete
	 */
	void CleanupGuards();

	/**
	 * Removes all larva marked for delete
	 */
	void CleanupLarva();

	static BeeManager* sInstance;
	std::vector<class OnlookerBee*> mOnlookers;
	std::vector<class EmployedBee*> mEmployees;
	std::vector<class QueenBee*> mQueens;
	std::vector<class Drone*> mDrones;
	std::vector<class Guard*> mGuards;
	std::vector<class Larva*> mLarva;

	const float FOOD_RETARGET_INTERVAL = 20.0f;
	float mTimeSinceRetarget;
	std::default_random_engine generator;
};

