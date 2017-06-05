#include "pch.h"
#include <iostream>
#include <windows.h>
#include <chrono>
#include "Bee.h"
#include "BeeManager.h"


using namespace std;
using namespace std::chrono;

const float FRAME_INTERVAL = 1.0f / 60.0f * 1000;

int main(int argc, char* argv[])
{
	// Allows console window to be shone for debugging, to display triggers or not otherwise rendered data points
#if _DEBUG
	ShowWindow(GetConsoleWindow(), SW_RESTORE);
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Hivemind", sf::Style::Fullscreen);

	float deltaTime = 0.0f;
	high_resolution_clock::time_point lastFrame = high_resolution_clock::now();
	const int beeRows = 25;
	const int beeCols = 25;

	BeeManager* beeManager = BeeManager::getInstance();

	for (int i = 0; i < beeRows; i++)
	{
		for (int j = 0; j < beeCols; j++)
		{
			beeManager->spawnBee(Bee(sf::Vector2f(200 + j * 55, 200 + i * 55)));
		}
	}

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
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{	// Check for manually closing simulation. This will later be the pause menu
			window.close();
		}

		deltaTime += duration_cast<seconds>(high_resolution_clock::now() - lastFrame).count();

		if (deltaTime >= FRAME_INTERVAL)
		{
			window.clear();
			beeManager->update(window, deltaTime);
			beeManager->render(window);
			deltaTime = 0.0f;
			window.display();
		}

	}

    return EXIT_SUCCESS;
}