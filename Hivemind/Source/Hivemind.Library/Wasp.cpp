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
	mState(State::Wandering), mGenerator(), mTargetHive(nullptr)
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

	if (mCollisionNode != nullptr && !mCollisionNode->ContainsPoint(mPosition))
	{	// If we haev a collision node and we leave it, invalidate the pointer
		mCollisionNode->UnregisterWasp(this);
		mCollisionNode = nullptr;
	}

	if (mCollisionNode == nullptr)
	{	// If the collision node is invalidated, get a new one and register to it
		mCollisionNode = CollisionGrid::GetInstance()->CollisionNodeFromPosition(mPosition);
		mCollisionNode->RegisterWasp(this);
	}
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

	auto hiveManager = HiveManager::GetInstance();
	for (auto iter = hiveManager->Begin(); iter != hiveManager->End(); ++iter)
	{
		auto target = (*iter)->GetCenterTarget();
		if (DistanceBetween(mPosition, target) < 500.0f)
		{
			mTargetHive = (*iter);
			mTarget = target;
			mState = State::Attacking;
			break;
		}
	}
}

void Wasp::UpdateAttackingState(sf::RenderWindow& window, const double& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);

	assert(mTargetHive != nullptr);

	float rotationRadians = atan2(mTarget.y - mPosition.y, mTarget.x - mPosition.x);
	mPosition.x += (cos(rotationRadians) * StandardWaspSpeed * deltaTime);
	mPosition.y += (sin(rotationRadians) * StandardWaspSpeed * deltaTime);

	if (DistanceBetween(mTarget, mPosition) <= Bee::TARGET_RADIUS)
	{
		auto dimensions = mTargetHive->GetDimensions();
		uniform_int_distribution<int> distributionX(static_cast<int>(-dimensions.x / 2), static_cast<int>(dimensions.x / 2));
		uniform_int_distribution<int> distributionY(static_cast<int>(-dimensions.y / 2), static_cast<int>(dimensions.y / 2));
		sf::Vector2f offset(static_cast<float>(distributionX(mGenerator)), static_cast<float>(distributionY(mGenerator)));
		mTarget = mTargetHive->GetCenterTarget() + offset;
	}
}
