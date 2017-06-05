#pragma once
#include "Bee.h"

class BeeManager
{
public:
	static BeeManager* getInstance();
	~BeeManager();
	void spawnBee(const Bee& bee);
	void update(sf::RenderWindow& window, const float& deltaTime);
	void render(sf::RenderWindow& window);
	std::vector<Bee>::iterator begin();
	std::vector<Bee>::iterator end();

private:
	BeeManager();
	static BeeManager* sInstance;
	std::vector<Bee> mBees;
};

