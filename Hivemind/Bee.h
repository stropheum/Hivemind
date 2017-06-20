#pragma once
#include "Entity.h"
#include <random>
#include <chrono>


class Hive;

class Bee : public Entity
{
public:

	enum State
	{
		SeekingTarget,
		HarvestingFood,
		DeliveringFood,
		DepositingFood,
	};

	const static float STANDARD_BEE_SPEED;
	const static float BODY_RADIUS;
	const static float TARGET_RADIUS;
	const static float STANDARD_HARVESTING_DURATION;
	const static sf::Color NORMAL_COLOR;
	const static sf::Color ALERT_COLOR;
	const static sf::Color Bee::STANDARD_BODY_COLOR;

	/// Constructors/destructor
	explicit Bee(const sf::Vector2f& position, const Hive& hive);
	virtual ~Bee() = default;

	/// Copy/Move semantics
	Bee(const Bee& rhs) = delete;
	Bee& operator=(const Bee& rhs) = delete;
	Bee(Bee&& rhs) = delete;
	Bee& operator=(Bee&& rhs) = delete;

	/// Public API
	void update(sf::RenderWindow& window, const float& deltaTime) override;
	void render(sf::RenderWindow& window) const override;
	bool collidingWithFoodSource(const class FoodSource& foodSource) const;
	void setColor(const sf::Color& color);
	void setTarget(const sf::Vector2f& position);
	const sf::Vector2f& getTarget() const;
	bool hasTarget() const;
	void setFont(sf::Font* const font);

private:
	/// Constants
	const float PI = 3.14159265359f;
	const float EXTRACTION_YIELD = 10.0f;

	/// Private API
	void handleFoodSourceCollisions();

	/// Private fields
	const Hive& mParentHive;
	std::default_random_engine mGenerator;
	sf::CircleShape mBody;
	sf::RectangleShape mFace;
	sf::Vector2f mTarget;
	float mSpeed;
	bool mTargeting;
	State mState;
	float mHarvestingDuration;
	sf::Clock mHarvestingClock;
	class FoodSource* mTargetFoodSource;
	float mFoodAmount;
	sf::Text mText;
	sf::Font mFont;
};
