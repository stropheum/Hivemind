#pragma once
#include "Bee.h"

/**
*	@Author: Dale Diaz
*	@Date: 7/25/2017
*/

class FooBee : public Bee
{
public:
	explicit FooBee(const sf::Vector2f& position, Hive& hive);
	~FooBee();
	void Update(sf::RenderWindow& window, const double& deltaTime) override;

protected:

	virtual void PopulateFunctionMaps() override {}
};

