#include "pch.h"
#include "Entity.h"


Entity::Entity(const sf::Vector2f& position, const sf::Color& outlineColor, const sf::Color& fillColor) :
	mPosition(position), mOutlineColor(outlineColor), mFillColor(fillColor), mCollisionNode(nullptr), mMarkedForDelete(false)
{}

float Entity::DistanceBetween(const sf::Vector2f& position_1, const sf::Vector2f& position_2)
{
	auto xDif = abs(position_1.x - position_2.x);
	auto yDif = abs(position_1.y - position_2.y);
	return sqrt((xDif * xDif) + (yDif * yDif));
}

void Entity::SetPosition(const sf::Vector2f& position)
{
	mPosition = position;
}

const sf::Vector2f& Entity::GetPosition() const
{
	return mPosition;
}

bool Entity::MarkedForDelete() const
{
	return mMarkedForDelete;
}

void Entity::MarkForDelete()
{
	mMarkedForDelete = true;
}
