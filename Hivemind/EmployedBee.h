#pragma once
#include "Bee.h"

namespace sf {
	class RenderWindow;
}

class EmployedBee :
	public Bee
{
public:

	EmployedBee(const sf::Vector2f& position, class Hive& hive);

	virtual ~EmployedBee() = default;

	void update(sf::RenderWindow& window, const float& deltaTime) override;
};

