#pragma once
#include <random>
#include <SFML/System/Vector2.hpp>

class PerlinNoise
{
public:
	PerlinNoise();
	~PerlinNoise();

	float** GenerateWhiteNoise(const sf::Vector2i& dimensions);
	
	float** generateSmoothNoise(float** values, const sf::Vector2i& dimensions, const int& octave);
	
	float** generatePerlinNoise(float** values, const sf::Vector2i& dimensions, const int& octaveCount);

private:
	static float interpolate(float x0, float x1, float alpha);

	std::default_random_engine mGenerator;
};

