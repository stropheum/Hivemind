#pragma once
#include <random>


class Hive;

class HiveManager
{
public:
	static HiveManager* GetInstance();
	~HiveManager();
	void SpawnHive(const sf::Vector2f& position);
	void Update(sf::RenderWindow& window, const float& deltaTime);
	void Render(sf::RenderWindow& window);
	std::vector<Hive*>::iterator Begin();
	std::vector<Hive*>::iterator End();

	/// Accessor method for hives based on the index of which they were added
	/// @Param index: The index of the hive being access
	/// @Exception: Thrown if accessing beyond the bounds of the vector
	Hive* GetHive(std::uint32_t index);

private:
	HiveManager();
	static HiveManager* sInstance;
	std::vector<Hive*> mHives;
	std::default_random_engine generator;
};

