#pragma once
#include <random>
#include <SFML/System/Vector2.hpp>


class PerlinNoise
{
public:

	/**
	 *  Constructor
	 */
	PerlinNoise();

	/**
	 *  Destructor
	 */
	~PerlinNoise() = default;

	/**
	 * Method which generates a base noise map
	 * @Param dimensions: The size of the noise map to be generated
	 * @Return: A 2D array of float values ranging from 0-1
	 */
	float** GenerateWhiteNoise(const sf::Vector2i& dimensions);

	/**
	 * Method for smoothing a noise map based on an octave value
	 * @Param value: The noise map being smoothed
	 * @Param dimensions: The size of the noise map
	 * @Param octave: The degree to which smoothing will occur
	 * @Return: A smoothed noise map of values ranging from 0-1
	 */
	float** GenerateSmoothNoise(float** values, const sf::Vector2i& dimensions, const int& octave);

	/**
	 * Method for generating a perlin noise map
	 * @Param value: The noise map being converted into perlin noise
	 * @Param dimensions: The size of the noise map
	 * @Param octaveCount: The number of different consecutive octaves being smoothed together
	 * @Return: A perlin noise map of values ranging from 0-1
	 */
	float** GeneratePerlinNoise(float** values, const sf::Vector2i& dimensions, const int& octaveCount);

private:

	/**
	 * Interpolates between two specified values
	 * @Param x0: The first value of the interpolation
	 * @Param x1: The second value of the interpolation
	 * @Param alpha: Specified alpha blending
	 * @Return: The value interpolated between x0 and x1
	 */
	static float Interpolate(float x0, float x1, float alpha);

	std::default_random_engine mGenerator;
};

