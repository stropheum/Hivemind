# Hivemind - A realistic bee colony simulation

Hivemind is a project I've started as an intersection of several interests of mine; AI programming, cellular automation, ecological simulations, and honey bees themselves. Since honey bees are such a crucial part of our ecosystem, and I hope to maintain a hive of my own in the near future, this project is a good opportunity to educate myself more about them while working on programming skills that interest me as well.

## Installation Instructions

If you are interested in running or modifying the simulation, there are a few steps you will have to take to make sure the dependencies for SFML are set up properly
1. [Download the SFML v2.4.2](https://www.sfml-dev.org/download/sfml/2.4.2/) You will need this build of SFML to run the visualization for the simulation. Either x86 or x64 will work, but x64 is recommended if possible
2. Navigate to the solution directory of the project. This should be the base Directory Hivemind/
3. If it does not exist, create a folder and name it "External". In that folder should be two additional folders titled "x64" and "x86", respectively. If they do not exist then create them as well.
4. Extract the SFML build into its associated platform directory in the External Folder. For example, if you installed the 64-bit version of SFML, then its path should be path-to-project/Hivemind/External/x64/SFML2.4.2/
5. Load the solution and rebuild. Make sure to select the project's platform that is associated with the version of SFML you wish to run

## Week 1 - The Artificial Bee Colony Algorithm

The first part of the simulation that I'm going to tackle is most likely the most important, and most complicated; simulating the foraging behavior of bees. After looking into established algorithms associated with bee foraging patterns, I discovered that there actually was one; the artificial bee colony algorithm, or "ABC algorithm" for short. The task I gave myself for this week was to set the basic groundwork for the project (integrating SFML, implementing a basic bee class), and researching the ABC algorithm in order to understand it at a level that I can accurately translate it to code.

Resources that I plan on using for this are the following:

[High-Level Overview of the algorithm](https://en.wikipedia.org/wiki/Artificial_bee_colony_algorithm)

[Document summarizing several logical implementations of the algorithm](http://mf.erciyes.edu.tr/abc/pub/NevImpOfABC.pdf)

So far, I understand the general principle that the algorithm works on - employed bees search for potential food sources, come back to the hive, transmit that food source information to onlookers, and after determining the food source with the highest fitness, some number of onlookers join the employed bee in harvesting from that food source. Rinse and repeat for the duration of the simulation, or until the hive dies.

The high-level overview of the algorithm is as follows:

- Initialize food sources for all "employed" bees
- Repeat until end of simulation  
  - Each employed bee scouts out a food source, determines the amount of nectar, and returns to the hive to waggle dance to convey that information to onlookers
  - Each onlooker watches the dances, determines the food source with the highest fitness, and seeks that out, while also evaluating neighbors for potential better fitness
  - Abandoned food sources are determined and fresh food sources will replace them as they are discovered by scout bees
  - The best food source currently discovered is registered
- End of simulation

As far as the basic logical implementation, I did a simple game loop, locking rendering to a fixed frame per second to allow for a more accurate visual simulation, while all business logic computations will be done via current delta time. I may decide to change this and have the business logic occur in fixed time steps if I find it better for an accurate reproducable simulation, but that is something I will address when the problem presents itself. The current bee API looks as follows:

```
#pragma once
class Bee
{
public:
	Bee();
	explicit Bee(const sf::Vector2f& position);
	~Bee() = default;
	void update(sf::RenderWindow& window);
	void render(sf::RenderWindow& window) const;
	void setPosition(const sf::Vector2f& position);
	const sf::Vector2f& getPosition() const;
	float getRadius() const;

private:
	const float mBodyRadius = 7.0f;
	const float pi = 3.14159265359;

	float distanceBetween(const sf::Vector2f& position_1, const sf::Vector2f& position_2) const;
	
	sf::CircleShape mBody;
	sf::RectangleShape mFace;
	sf::Vector2f mPosition;
	float speed;
};
```
This is obviously just a first rough pass, but it has the basic implementation of what I need; a constructor with position(for adding bees dynamically to a beemanager aggregate class), an update for business logic, a render method, and setters/getters for relevant simulation information. It also has a physical body and a face to show which direction the bee is facing, which will be useful later on, as it will be hard to judge the "intent" of any given bee at a glance, or if the simulation is paused, but with a line always pointing towards its intended direction, the visual simplicity is greatly increased.


Here you can see a small swarm of bees converging on a mouse pointer in the current simulation
![Simple Bee Swarm](http://i.imgur.com/bXtLkiJ.png "Simple bee swarm")

In the coming week, I hope to have the basic implementation of the ABC algorithm implemented - implemented food sources, employed bees scouting, waggle dancing, and onlooker food source evaluation are my primary goals, as well as on the visualization end, I hope to get a more realistic movement system for the bees where they collide to avoid perfect stacking on one another, but still have that swarming aspect where they will push around each other to get as close to the destination as possible.
