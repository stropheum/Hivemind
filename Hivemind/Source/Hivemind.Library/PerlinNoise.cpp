#include "pch.h"
#include "PerlinNoise.h"
#include <chrono>

/**
*	@Author: Dale Diaz
*	@Date: 7/21/2017
*/

PerlinNoise::PerlinNoise()
{
	std::random_device device;
	mGenerator = std::default_random_engine(device());
}

float** PerlinNoise::GenerateWhiteNoise(const sf::Vector2i& dimensions)
{
	float** noise = new float*[dimensions.x];
	for (int i = 0; i < dimensions.x; i++)
	{
		noise[i] = new float[dimensions.y];
	}

	std::uniform_real_distribution<float> distribution(0, 1);

	for (int i = 0; i < dimensions.y; i++)
	{
		for (int j = 0; j < dimensions.x; j++)
		{
			noise[i][j] = 0;
		}
	}

	for (int i = 0; i < dimensions.x; i++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			noise[i][j] = distribution(mGenerator);
		}
	}

	return noise;
}

float** PerlinNoise::GenerateSmoothNoise(float** values, const sf::Vector2i& dimensions, const int& octave)
{
	float** smoothNoise = new float*[dimensions.x];
	for (int i = 0; i < dimensions.x; i++)
	{
		smoothNoise[i] = new float[dimensions.y];
	}

	int samplePeriod = 1 << octave; // calculates 2 ^ k
	float sampleFrequency = 1.0f / samplePeriod;

	for (int i = 0; i < dimensions.x; i++)
	{
		//calculate the horizontal sampling indices
		int sample_i0 = (i / samplePeriod) * samplePeriod;
		int sample_i1 = (sample_i0 + samplePeriod) % dimensions.x; //wrap around
		float horizontal_blend = (i - sample_i0) * sampleFrequency;

		for (int j = 0; j < dimensions.y; j++)
		{
			//calculate the vertical sampling indices
			int sample_j0 = (j / samplePeriod) * samplePeriod;
			int sample_j1 = (sample_j0 + samplePeriod) % dimensions.y; //wrap around
			float vertical_blend = (j - sample_j0) * sampleFrequency;

			//blend the top two corners
			float top = Interpolate(values[sample_i0][sample_j0],
				values[sample_i1][sample_j0], horizontal_blend);

			//blend the bottom two corners
			float bottom = Interpolate(values[sample_i0][sample_j1],
				values[sample_i1][sample_j1], horizontal_blend);

			//final blend
			smoothNoise[i][j] = Interpolate(top, bottom, vertical_blend);
		}
	}

	return smoothNoise;
}

float** PerlinNoise::GeneratePerlinNoise(float** values, const sf::Vector2i& dimensions, const int& octaveCount)
{
	float*** smoothNoise = new float**[octaveCount];

	float persistance = 0.5f;

	//generate smooth noise
	for (int i = 0; i < octaveCount; i++)
	{
		smoothNoise[i] = GenerateSmoothNoise(values, dimensions, i);
	}

	float** perlinNoise = new float*[dimensions.x];
	for (int i = 0; i < dimensions.x; i++)
	{
		perlinNoise[i] = new float[dimensions.y];
		for (int j = 0; j < dimensions.y; j++)
		{
			perlinNoise[i][j] = 0;
		}
	}

	float amplitude = 1.0f;
	float totalAmplitude = 0.0f;

	//blend noise together
	for (int octave = octaveCount - 1; octave >= 0; octave--)
	{
		amplitude *= persistance;
		totalAmplitude += amplitude;

		for (int i = 0; i < dimensions.x; i++)
		{
			for (int j = 0; j < dimensions.y; j++)
			{
				perlinNoise[i][j] += smoothNoise[octave][i][j] * amplitude;
			}
		}
	}

	//normalisation
	for (int i = 0; i < dimensions.x; i++)
	{
		for (int j = 0; j < dimensions.y; j++)
		{
			perlinNoise[i][j] /= totalAmplitude;
		}
	}

	for (int i = 0; i < octaveCount; i++)
	{
		for (int j = 0; j < dimensions.x; j++)
		{
			delete[] smoothNoise[i][j];
		}
		delete[] smoothNoise[i];
	}
	delete[] smoothNoise;

	return perlinNoise;
}

float PerlinNoise::Interpolate(float x0, float x1, float alpha)
{
	return x0 * (1 - alpha) + alpha * x1;
}
