#include "pch.h"
#include "FooEntity.h"


FooEntity::FooEntity(const sf::Vector2f& position, const sf::Color& outlineColor, const sf::Color& fillColor):
	Entity(position, outlineColor, fillColor)
{
}

void FooEntity::Update(sf::RenderWindow& window, const float& deltaTime)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(deltaTime);
}

void FooEntity::Render(sf::RenderWindow& window) const
{
	UNREFERENCED_PARAMETER(window);
}
