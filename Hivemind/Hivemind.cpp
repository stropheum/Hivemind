#include "pch.h"
#include <iostream>
#include <windows.h>
#include "Bee.h"


using namespace std;

int main(int argc, char* argv[])
{
	// Allows console window to be shone for debugging, to display triggers or not otherwise rendered data points
#if _DEBUG
	ShowWindow(GetConsoleWindow(), SW_RESTORE);
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Hivemind");

	const int beeRows = 50;
	const int beeCols = 50;
	Bee bee[beeRows][beeCols];

	for (int i = 0; i < beeRows; i++)
	{
		for (int j = 0; j < beeCols; j++)
		{
			auto positionOffset = bee[i][j].getRadius() * 2 + 25;
			bee[i][j].setPosition(sf::Vector2f(positionOffset * j, positionOffset * i));
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

		window.clear();
		
		for (int i = 0; i < beeRows; i++)
		{
			for (int j = 0; j < beeCols; j++)
			{
				bee[i][j].update(window);
				bee[i][j].render(window);
			}
		}

		window.display();
	}

    return EXIT_SUCCESS;
}

