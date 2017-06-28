#include "pch.h"
#include "PerlinNoise.h"
#include <chrono>


//std::default_random_engine PerlinNoise::mGenerator(std::random_device());

PerlinNoise::PerlinNoise()
{
//	mGenerator.seed(static_cast<long>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
	std::random_device device;
	mGenerator = std::default_random_engine(device());
}


PerlinNoise::~PerlinNoise()
{
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

float** PerlinNoise::generateSmoothNoise(float** values, const sf::Vector2i& dimensions, const int& octave)
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
			float top = interpolate(values[sample_i0][sample_j0],
				values[sample_i1][sample_j0], horizontal_blend);

			//blend the bottom two corners
			float bottom = interpolate(values[sample_i0][sample_j1],
				values[sample_i1][sample_j1], horizontal_blend);

			//final blend
			smoothNoise[i][j] = interpolate(top, bottom, vertical_blend);
		}
	}

	return smoothNoise;
}

float** PerlinNoise::generatePerlinNoise(float** values, const sf::Vector2i& dimensions, const int& octaveCount)
{
	float*** smoothNoise = new float**[octaveCount];

	float persistance = 0.5f;

	//generate smooth noise
	for (int i = 0; i < octaveCount; i++)
	{
		smoothNoise[i] = generateSmoothNoise(values, dimensions, i);
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

float PerlinNoise::interpolate(float x0, float x1, float alpha)
{
	return x0 * (1 - alpha) + alpha * x1;
}
