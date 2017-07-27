#include "pch.h"
#include "Wasp.h"

/**
 * @Author: Dale Diaz
 * @Date: 7/26/2017
 */

using namespace std;

const float Wasp::StandardWaspSpeed = 300.0f;

Wasp::Wasp(const sf::Vector2f& position):
	Entity(position, sf::Color(196, 196, 196), sf::Color::Red),
	mState(State::Wandering), mGenerator()
{
	std::random_device device;
	mGenerator = std::default_random_engine(device());
	GenerateNewTarget();

	mBody.setRadius(Bee::BodyRadius);
	mBody.setFillColor(mFillColor);
	mBody.setOutlineColor(mOutlineColor);
	mBody.setPosition(mPosition);
	mBody.setOutlineThickness(-2);
}

Wasp::~Wasp()
{
}

void Wasp::Update(sf::RenderWindow& window, const double& deltaTime)
{
	switch (mState)
	{
		case State::Wandering:
			UpdateWanderingState(window, deltaTime);
			break;
		case State::Attacking:
			UpdateAttackingState(window, deltaTime);
			break;
	}

	mBody.setPosition(mPosition);
}

void Wasp::Render(sf::RenderWindow& window) const
{
	window.draw(mBody);
}

void Wasp::GenerateNewTarget()
{
	uniform_real_distribution<float> distribution(-500.0f, 500.0f);
	sf::Vector2f offset(distribution(mGenerator), distribution(mGenerator));
	mTarget = mPosition + offset;
}

void Wasp::UpdateWanderingState(sf::RenderWindow& window, const double& deltaTime)
{
	UNREFERENCED_PARAMETER(window);

	float rotationRadians = atan2(mTarget.y - mPosition.y, mTarget.x - mPosition.x);
	mPosition.x += (cos(rotationRadians) * StandardWaspSpeed * deltaTime);
	mPosition.y += (sin(rotationRadians) * StandardWaspSpeed * deltaTime);

	if (DistanceBetween(mPosition, mTarget) <= mBody.getRadius())
	{
		GenerateNewTarget();
	}
}

void Wasp::UpdateAttackingState(sf::RenderWindow& window, const double& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);
}
