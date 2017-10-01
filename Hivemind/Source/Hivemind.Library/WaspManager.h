#pragma once


class WaspManager
{
public:

	/**
	 * Destructor
	 */
	~WaspManager();

	/**
	 * Singleton accessor method
	 * @Return: A pointer to the WaspManager instance
	 */
	static WaspManager* GetInstance();

	/**
	 * Accessor for the begin iterator of the wasp vector
	 * @Return: An iterator pointing to the beginning of the wasps list
	 */
	std::vector<Wasp*>::iterator Begin();

	/**
	* Accessor for the end iterator of the wasp vector
	* @Return: An iterator pointing to the end of the wasps list
	*/
	std::vector<Wasp*>::iterator End();

	/**
	 * Disseminates update calls to all spawned wasps
	 */
	void Update(sf::RenderWindow& window, const double& deltaTime);

	/**
	 * Disseminates render calls to all spawned wasps
	 */
	void Render(sf::RenderWindow& window) const;

	/**
	 * Spawns a wasp at the specified location
	 */
	void SpawnWasp(const sf::Vector2f& position);

	/**
	 * Destroys a wasp and removes it from the list, if it exists
	 * @Param wasp: The wasp being destroyed
	 */
	void DestroyWasp(Wasp* const wasp);

private:

	/**
	 * Constructor
	 */
	WaspManager();

	/**
	 * Iterates over all wasps and removes them if they are marked for delete
	 */
	void CleanupWasps();
	
	static WaspManager* sInstance;

	std::vector<Wasp*> mWasps;
	const float mSpawnInterval = 5.0f;
	std::default_random_engine mGenerator;
	float mTimeSinceSpawn;
};

