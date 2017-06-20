#pragma once
#include <random>


class Hive;

class HiveManager
{
public:
	static HiveManager* getInstance();
	~HiveManager();
	void spawnHive(const sf::Vector2f& position);
	void update(sf::RenderWindow& window, const float& deltaTime);
	void render(sf::RenderWindow& window);
	std::vector<Hive*>::iterator begin();
	std::vector<Hive*>::iterator end();

	/// Accessor method for hives based on the index of which they were added
	/// @Param index: The index of the hive being access
	/// @Exception: Thrown if accessing beyond the bounds of the vector
	Hive* getHive(std::uint32_t index);

private:
	HiveManager();
	static HiveManager* sInstance;
	std::vector<Hive*> mHives;
	std::default_random_engine generator;
};

