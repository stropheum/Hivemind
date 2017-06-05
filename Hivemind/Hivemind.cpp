#include "pch.h"
#include <iostream>
#include <windows.h>
#include <chrono>
#include <sstream>
#include "Bee.h"
#include "BeeManager.h"
#include "FoodSource.h"
#include "FoodSourceManager.h"


using namespace std;
using namespace std::chrono;

const float FRAME_INTERVAL = 1.0f / 60.0f;

string computeFrameRate();

int main(int argc, char* argv[])
{
	// Allows console window to be shone for debugging, to display triggers or not otherwise rendered data points
#if _DEBUG
	ShowWindow(GetConsoleWindow(), SW_RESTORE);
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Hivemind", sf::Style::Fullscreen);
	sf::Text fpsMeter;
	sf::Font font;
	font.loadFromFile("Hack-Regular.ttf");
	fpsMeter.setFont(font);
	fpsMeter.setCharacterSize(16);
	fpsMeter.setPosition(0, 0);
	fpsMeter.setFillColor(sf::Color(200, 200, 200));

	bool running = false;
	float deltaTime = 0.0f;
	high_resolution_clock::time_point lastFrame = high_resolution_clock::now();
	const int beeRows = 25;
	const int beeCols = 25;
	const int horizontalSpacing = window.getSize().x / beeCols;
	const int verticalSpacing = window.getSize().y / beeRows;

	BeeManager* beeManager = BeeManager::getInstance();
	FoodSourceManager* foodSourceManager = FoodSourceManager::getInstance();

	for (int i = 0; i < beeRows; i++)
	{
		for (int j = 0; j < beeCols; j++)
		{	// Distribute bees evenly across the screen
			beeManager->spawnBee(Bee(sf::Vector2f(horizontalSpacing / 2 + horizontalSpacing * j, verticalSpacing / 2 + verticalSpacing * i)));
		}
	}

	auto windowSize = window.getSize();
	foodSourceManager->spawnFoodSource(sf::Vector2f(windowSize.x / 2 - 50, windowSize.y / 2 - 50));
	foodSourceManager->spawnFoodSource(sf::Vector2f(windowSize.x / 4 - 50, windowSize.y / 4 - 50));
	foodSourceManager->spawnFoodSource(sf::Vector2f(windowSize.x - windowSize.x / 4 - 50, windowSize.y / 4 - 50));
	foodSourceManager->spawnFoodSource(sf::Vector2f(windowSize.x / 4 - 50, windowSize.y -  windowSize.y / 4 - 50));
	foodSourceManager->spawnFoodSource(sf::Vector2f(windowSize.x - windowSize.x / 4 - 50, windowSize.y - windowSize.y / 4 - 50));

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::Resized)
			{
				window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					running = !running;
				}
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{	// Check for manually closing simulation. This will later be the pause menu
			window.close();
		}

		deltaTime += (duration_cast<milliseconds>(high_resolution_clock::now() - lastFrame).count() / 1000.0f);

		if (deltaTime > FRAME_INTERVAL)
		{
			// Reset last frame so time isn't dilated between updates
			lastFrame = high_resolution_clock::now();

			// Handle business logic updates
			if (running)
			{
				beeManager->update(window, deltaTime);
			}

			// Handle rendering
			window.clear();

			fpsMeter.setString(computeFrameRate());
			window.draw(fpsMeter);

			// Rendering temporary food source
			foodSourceManager->render(window);

			beeManager->render(window);

			window.display();

			// Reset delta time
			deltaTime = 0.0f;
		}
		
	}

    return EXIT_SUCCESS;
}

string computeFrameRate()
{
	static const int MAX_FRAMES = 10000;
	static high_resolution_clock::time_point startTime = high_resolution_clock::now();
	static uint32_t frameCount = 0;
	frameCount++;
	
	if (frameCount >= MAX_FRAMES)
	{	// If we surpass max frames then we reset the FPS counter
		startTime = high_resolution_clock::now();
		frameCount = 0;
	}

	int timeSinceStart = duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() / 1000.0f;

	string result = timeSinceStart != 0 ? to_string(frameCount / timeSinceStart) : "0";

	stringstream ss;
	ss << "frameCount: " << frameCount << endl;
	ss << "time since start: " << timeSinceStart << endl;
	ss << "fps: " << result << endl << endl;

	return ss.str();
}