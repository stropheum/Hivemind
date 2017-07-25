#include "pch.h"
#include "FooEntity.h"

/**
*	@Author: Dale Diaz
*	@Date: 7/25/2017
*/

FooEntity::FooEntity(const sf::Vector2f& position, const sf::Color& outlineColor, const sf::Color& fillColor):
	Entity(position, outlineColor, fillColor)
{
}

void FooEntity::Update(sf::RenderWindow& window, const double& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);
}

void FooEntity::Render(sf::RenderWindow& window) const
{
	UNREFERENCED_PARAMETER(window);
}
