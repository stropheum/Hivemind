#include "pch.h"

using namespace std;
using namespace std::chrono;

/**

	██╗  ██╗██╗██╗   ██╗███████╗███╗   ███╗██╗███╗   ██╗██████╗
	██║  ██║██║██║   ██║██╔════╝████╗ ████║██║████╗  ██║██╔══██╗
	███████║██║██║   ██║█████╗  ██╔████╔██║██║██╔██╗ ██║██║  ██║
	██╔══██║██║╚██╗ ██╔╝██╔══╝  ██║╚██╔╝██║██║██║╚██╗██║██║  ██║
	██║  ██║██║ ╚████╔╝ ███████╗██║ ╚═╝ ██║██║██║ ╚████║██████╔╝
	╚═╝  ╚═╝╚═╝  ╚═══╝  ╚══════╝╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═════╝

	A simulation intended to represent the organic behavior of bees, predominantly focused on the foraging and communication patterns of bees.
	Each bee is assigned a role at birth which will dictate their behavioral patterns and physical abilities. Beehives will attempt to reach
	a homeostatic balance with their environment, ie continually seeking higher yield food sources, which not only allows a food source to
	replenish, but also allows the hive to maintain more efficient harvests and stockpile honey more rapidly.

	The foraging and communication patterns have been modeled after the "Artificial Bee Colony (ABC)" Algorithm developed by Karaboga in 2005

	@Author: Dale Diaz
	@Date: 7/21/2017

*/

const float FRAME_INTERVAL = 1.0f / 120.0f;
const float CAMERA_SPEED = 350.0f;
sf::Clock deltaClock;
sf::Clock uiDeltaClock;

string computeFrameRate();

int main(int argc, char* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
	// Allows console window to be shown for debugging, to display triggers or not-otherwise rendered data points
#if _DEBUG
	ShowWindow(GetConsoleWindow(), SW_RESTORE);
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 16;

	sf::View view(sf::FloatRect(0, 0, 1600, 900));
	float totalZoom = 1.25f;
	view.zoom(totalZoom);
	sf::Vector2f cameraMovement(0, 0);

	sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Hivemind", sf::Style::Default);
	window.setView(view);
	window.setSize(sf::Vector2u(1600, 900));
	view.setCenter(1000, 1000);
	window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - window.getSize().x / 2,
		sf::VideoMode::getDesktopMode().height / 2 - window.getSize().y / 2));
	
	sf::Text fpsMeter;
	sf::Font font;
	font.loadFromFile("Hack-Regular.ttf");
	fpsMeter.setFont(font);
	fpsMeter.setCharacterSize(16);
	fpsMeter.setPosition(0, 0);
	fpsMeter.setFillColor(sf::Color(200, 200, 200));
	
	FlowFieldManager::GetInstance();
	auto beeManager = BeeManager::GetInstance();
	auto hiveManager = HiveManager::GetInstance();
	auto foodSourceManager = FoodSourceManager::GetInstance();
	auto collisionGrid = CollisionGrid::GetInstance();

	string worldConfig = argc >= 2 ? argv[1] : "big_world.json";
	WorldGenerator::GetInstance()->Generate(worldConfig);
	view.setCenter(HiveManager::GetInstance()->GetHive(0)->GetCenterTarget());

	bool running = false;
	deltaClock.restart();
	uiDeltaClock.restart();

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
				// Handle resize-specific logic
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Numpad1)
				{
					collisionGrid->ToggleGridVisualization();
				}
				if (event.key.code == sf::Keyboard::Numpad2)
				{
					beeManager->ToggleEmployeeFlowFields();
				}

				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
				{
					cameraMovement.x = -CAMERA_SPEED;
				}
				else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
				{
					cameraMovement.x = CAMERA_SPEED;
				}
				else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
				{
					cameraMovement.y = -CAMERA_SPEED;
				}
				else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
				{
					cameraMovement.y = CAMERA_SPEED;
				}
				else
				{
					cameraMovement = sf::Vector2f(0, 0);
				}

				fpsMeter.setPosition(
					sf::Vector2f(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2));
				window.setView(view);
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
				{
					cameraMovement.x = 0;
				}
				else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
				{
					cameraMovement.x = 0;
				}
				else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
				{
					cameraMovement.y = 0;
				}
				else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
				{
					cameraMovement.y = 0;
				}

				if (event.key.code == sf::Keyboard::Space)
				{
					running = !running;
					deltaClock.restart();
				}
			}

			if (event.type == sf::Event::MouseWheelScrolled)
			{
				float scaleFactor = 1.0f - (2 * event.mouseWheelScroll.delta / 100.0f);
				if (totalZoom - 2 * event.mouseWheelScroll.delta / 100.0f > 0.0f)
				{
					totalZoom -= 2 * event.mouseWheelScroll.delta / 100.0f;
				}
				
				view.zoom(scaleFactor);
				window.setView(view);
				fpsMeter.scale(scaleFactor, scaleFactor);
				fpsMeter.setPosition(
					sf::Vector2f(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2));
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{	// Check for manually closing simulation. This will later be the pause menu
			window.close();
		}

		// Handle business logic updates
		if (running)
		{
			auto deltaTime = deltaClock.restart().asSeconds();
			hiveManager->Update(window, deltaTime);
			beeManager->Update(window, deltaTime);
			foodSourceManager->Update(window, deltaTime); 
		}

		auto uiDeltaTime = uiDeltaClock.restart().asSeconds();
		view.move(cameraMovement * totalZoom * uiDeltaTime);

		// Handle rendering
		window.clear(sf::Color(32, 32, 32));

		fpsMeter.setPosition(
			sf::Vector2f(view.getCenter().x - view.getSize().x / 2, view.getCenter().y - view.getSize().y / 2));
		window.setView(view);
		fpsMeter.setString(computeFrameRate());
		window.draw(fpsMeter);

		hiveManager->Render(window);
		foodSourceManager->Render(window);
		beeManager->Render(window);
		collisionGrid->Render(window);

		window.display();
		
	}

    return EXIT_SUCCESS;
}

string computeFrameRate()
{
	stringstream ss;
	static const int MAX_FRAMES = 1000000;
	static high_resolution_clock::time_point startTime = high_resolution_clock::now();
	static uint32_t frameCount = 0;
	frameCount++;

	int timeSinceStart = static_cast<int>(duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() / 1000.0f);

	string result = timeSinceStart != 0 ? to_string(frameCount / timeSinceStart) : "0";
	ss << "FPS: " << result << endl << endl;

	if (frameCount >= MAX_FRAMES)
	{	// If we surpass max frames then we reset the FPS counter
		startTime = high_resolution_clock::now();
		frameCount = 0;
	}

	return ss.str();
}