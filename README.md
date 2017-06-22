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

## Week 2 - The Pursuit of Foraging

I've now gotten basic movement working and the concept of a target roughly implemented, so the next goal is to create real objects in the simulation space for the bees to target and travel to. To start this, I created a food source class and an associated food source manager singleton, which would be responsible for creating and managing all of the food sources, similar to the Bee/BeeManager singleton relationship that already exists. Along with refactoring the UI to be a little easier on the eyes, I modified the Bee class to reassign itself a new target once its current target is reached

Below you can see a video of the current simulation in progress.

[![Hivemind Update 6-6](https://img.youtube.com/vi/6uNdDS5cils/0.jpg)](https://youtu.be/6uNdDS5cils)
- The Bees have been modified to have an outer skin which makes them stand out more clearly against all backgrounds as well as being colorable. This will allow me to visually display status changes without cluttering the screen
- The Green Squares are food sources. Currently they can take up any quadrilateral space, but I've defaulted them to squares for now
- When a bee collides with a food source, his body changes color to red to indicate that he is within the bounds of the food source
- Once a bee reaches its target, it randomly selects a new food source and runs to that
- To progress from here, I will need to do the following:
	- Implement a hive instead of a food source at the center
	- Create a "wandering" idle algorithm for bees so they will putter around the hive or food source until their task is complete
	- Factor in more robust logic, ie -> forage until either food capacity of the bee ix maxed or until the food source runs out, then return to the hive, and spend x amount of time depositing the food
	- Establish the concept of food in hives as well as food sources
	
### Foraging (continued)

After a lot of tweaking, refactoring, and organizing the code to be a little bit more logical and performant (mostly moving all logical functionality out of manager and into the classes themselves), as well as creating a base entity class to get rid of a fair bit of boilerplate that was starting to crop up among all of my simulation entities, I now have a rudimentary foraging algorithm working. 

Bees will now start off in a "seeking" state, and will randomly choose a food source (later on this information will be second-hand from employed bees, but proof of concept for now). Once the bees reach the center of their food source, they will enter a harvesting state, where they will remain for a set period of time before seeking out additional food sources. The later implementation will involve a "wandering" algorithm so they actually appear to be active on the food source, and will return to the hive with their food deposits. 

As you can see in the video below, there is also a visual aid in a red barrier to denote when a bee is colliding and interacting with world objects. I feel like this will be a standard indicator that I use hereon out to denote the state of individual bees, as it seems very clear to discern what any given bee is doing at any given moment.

## Week 3 - The Beginnings of a Hive

So now I'm beginning to get in the thick of things. Not only am I trying to set up the actual simulation aspects of the hive, but I'm also attempting to do a lot of busy work that will provide me with live debugging tools down the road. 

This week waas mostly geared twoards the chore mechanics as well as fleshing out some controls and data management. From a high-level, my tasks were the following:
- Implement a wandering AI for harvesting bees to look busy while active on a food source
- Implement the core hive system. This means food sources have food, bees can harvest that food and bring it to the hive
- Divide bees into employed/scouts/onlookers, and start the process of disseminating tasks between them
- Expand UI controls. This entails basically getting the camera controls up and running as well as getting some display information in the game

The first thing I tackled was the wandering AI, mostly because the idle aesthethic was really bugging me (no pun intended), and I wanted to up the organic feel of the simulation.
What I did to approach this was to use the current targeting system that I had already implemented for targeting and pursuing a food source, and while in the harvesting state, it will choose a random point within the bound of the square and pursue it. When the bee encounters this new idle-target, if it still has harvesting time left, it will retarget another random point. Rinse and repeat until its harvesting timer has completed, and then it selects another random food source after packing on a healthy dose of food, which is then extracted from the food source's reserves. The food source will get visually dimmer as a reflection of its current food capacity until it becomes fully dark.

This task obviously bled over into my second task, which was implementing the core hive system. Getting the bees to behave organically naturally flowed into them behaving accurately, and since I was already working on the logic related to bees and food sources, it felt natural to implement the food system aspect while I was going.

After the food system was implemented, I decided it would be handy to get some debug information going real-time so I didn't have to deal with hunting down breakpoints just to get some feedback on the status of individual bees. So for this, I added a text component to all of the bees and when I decide for it to be active, they can display whatever data I deem relevent; obviously for now it's just food, but later on health and hunger are also important values to track.

Next for me was upping the degree of control I have. The more freedom to move around the environment, the more I can explore and play around with aspects of the simulation that leverage aspects like variable distance, travel time, the affects of hunger on long journeys, etc. Essentially it was such a useful feature that I felt like I needed to tackle it ASAP. This was relatively simple, as all that was involved was listening for key presses and moving the camera view a set amount, but allowing for simultaneously smooth vertical and horizontal movement was a bit of an issue, as well as making sure all UI components are still rendering in the proper position. For those of you who aren't familiar with SFML, there is no strict "UI" layer to render to, but rather you just have to remain cognizant as a developer of the differences between screen and world space, and one cannot rely simply on assuming that the position you render text will be the actual screen position that the text gets rendered to.

### Bugs. Ugh.
So I screwed something up, for sure. The foraging algorithm thusfar seems to be working as well as it has been before, but the data overlay revealed a problem to me that I hadn't noticed before; The bees aren't always foraging once they hit their target food source. I noticed that it seems somewhat arbitrary, but sometimes a bee will reach its food source, decide for whatever reason that he doesn't like it, and then move on to another one without entering its harvesting phase. This was quite strange and I couldn't manage to track it down in time to fix it for this update, and it ate into my time to split the bees into their respective roles, so that stinks. I do plan on keeping up to schedule, but that means I will have to commit a little extra time in the coming week, which means less sanity, or more alcohol. Stay tuned for picture and video updates!

## Week 4 - The Key to any Hive: Communication
This is where things start to get exciting! My goals for this week are to get the beginnings of communication working in my hive; I want hives, food sources, and bees to all be able to store and know about food, and to have certain bees able to communicate to others to start the makings of the actual bee-communication behavior.

Starting off, I decided to tackle a lot of the hive components. This involved making things a lot more complex, so I figured getting it out of the way was the wisest move. Not only did the hive have to track the bees the were idling in it, but it also needed to be capable of disseminating information to them. My solution to this was to create a private vector of OnlookerBee pointers and public accessors for its iterators, allowing external sources to send messages to the bees en masse without mucking with the vector itself; since the BeeManager, not the hive, owns the pointers to these bees, keeping them under the hood seemed the most practical approach

```
class Hive :
	public Entity
{
public:
	/// Constructor
	/// @Param position: The starting position of the food source
	explicit Hive(const sf::Vector2f& position);
	
	/// Destructor
	virtual ~Hive();

	/// Updates the current game state of the hive
	/// @Param window: The screen that the game is being rendered to
	/// @Param deltaTime: The time elapsed since last update
	void update(sf::RenderWindow& window, const float& deltaTime) override;
	
	/// Renders the hive to the screen
	/// @Param window: The screen which the hive is being rendered to
	void render(sf::RenderWindow& window) const override;

	/// Accessor method for the center point of the food source
	/// @Return: A vector representing the center point of the source
	sf::Vector2f getCenterTarget() const;

	/// Accessor method for the dimensions of the food source
	/// @Return: A vector representing the width and height of the food source
	const sf::Vector2f& getDimensions() const;
	
	/// Adds food to the hive
	/// @Param foodAmount: Amount of food being added to the hive
	void depositFood(const float& foodAmount);

	/// Adds an onlooker to the hive for easy tracking during waggle dance phase
	/// @Param bee: Bee being added as idle to the hive
	void addIdleBee(OnlookerBee* const bee);

	/// Removes an onlooker from the hives collection of idle bees
	/// @Param bee: Bee being removed from the collection of idle bees
	void removeIdleBee(OnlookerBee* const bee);

	/// Accessor for the begin iterator of the idle bees
	/// @Return: An iterator pointing to the beginning of the idle bees vector
	std::vector<OnlookerBee*>::iterator idleBeesBegin();

	/// Accessor for the end iterator of the idle bees
	/// @Return: An iterator pointing to the end of the idle bees vector
	std::vector<OnlookerBee*>::iterator idleBeesEnd();
	
	/// Iterates over the idle bees vector and removes all bees which are no longer idle
	void validateIdleBees();

private:
	/// Constants
	const float STANDARD_WIDTH = 200.0f;
	const float STANDARD_HEIGHT = 200.0f;

	/// Fields
	sf::Vector2f mDimensions;
	sf::RectangleShape mBody;
	float mFoodAmount;
	sf::Font mFont;
	sf::Text mText;
	std::vector<OnlookerBee*> mIdleBees;
};
```

In addition, you can see that I've also added a "validateIdleBees()" method, which at the moment was a simple solution to remove active bees from the vector without invalidating the external iterators during iteration.

In terms of the bee behavior itself, I now required the existence of an "idle" state, which allowed the bees to stay dormant and wait for external commands to stimulate them to pursue a fresh food source. This was the basic imlpementation once I got the bees to Idle in-hive
[![Hivemind Idle 6-21](https://img.youtube.com/vi/IAiB2aqflI8/0.jpg)](https://www.youtube.com/watch?v=IAiB2aqflI8)

You can see from the video that instead of "seeking target" as their initial state, bees now default to "deliver food" which causes them to pursue the center of the hive, where they cascade into "deliver food" which will dump their reserves into the hive (if any) and then enter their idle state, where they will wander and wait instructions.

The next task from here was naturally to develop the scouting/employed bee, which would seek out a food source, gather a bit of food, return to the hive with that information, and attempt to persuade the idle bees to pursue that as their active food source. You can view a demo of the basic implementation of that behavior here:
[![Hivemind Employed Bee Demo](https://youtu.be/w_m1j3X1hvY)
[![Hivemind Employed Bee Demo](https://img.youtube.com/vi/w_m1j3X1hvY/0.jpg)](https://www.youtube.com/watch?v=w_m1j3X1hvY)

And that's about it. There were a lot of minor optimizations that I had to do this week regarding how to store references, minimizing iterations over collections, managing data lifetimes, and the like, which ate significantly into my time specifically regarding the hive implementation. I intended to get started with working on the structural aspects of the hive such as the worker bees, implementing brood/honey/structural comb, etc, but I simply did not get to it in time, so that will carry over into week 5, where I focus mostly on the structural aspects of the hive, and breeding, as well as refining the decision making process of the bees, and how the food source selection happens, rather than by simple random chance. More to come!
