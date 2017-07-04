#pragma once
#include "Bee.h"
#include <SFML/System/Vector2.hpp>

class OnlookerBee :
	public Bee
{
public:
	OnlookerBee(const sf::Vector2f& position, Hive& hive);
	virtual ~OnlookerBee() = default;

	void Update(sf::RenderWindow& window, const float& deltaTime) override;
};

