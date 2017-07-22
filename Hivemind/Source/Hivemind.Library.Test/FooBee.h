#pragma once
#include "Bee.h"

class FooBee : public Bee
{
public:
	explicit FooBee(const sf::Vector2f& position, Hive& hive);
	~FooBee();
	void Update(sf::RenderWindow& window, const float& deltaTime) override;
};

