#pragma once
#include "Bee.h"
#include "Flowfield.h"

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
	
	void render(sf::RenderWindow& window) const override;

	void toggleFlowField();

private:

	void waggleDance();

	class FoodSource* mPairedFoodSource;
	FlowField mFlowField;
	bool mDisplayFlowField;
};

