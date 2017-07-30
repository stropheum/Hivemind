# Hivemind - A realistic bee colony simulation

Hivemind is a project I've started as an intersection of several interests of mine; AI programming, cellular automation, ecological simulations, and honey bees themselves. Since honey bees are such a crucial part of our ecosystem, and I hope to maintain a hive of my own in the near future, this project is a good opportunity to educate myself more about them while working on programming skills that interest me as well.

## Installation Instructions

If you are interested in running or modifying the simulation, there are a few steps you will have to take to make sure the dependencies for SFML are set up properly
1. [Download SFML v2.4.2](https://www.sfml-dev.org/download/sfml/2.4.2/) You will need this build of SFML to run the visualization for the simulation. Either x86 or x64 will work, but x64 is recommended if possible
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

[![Hivemind Employed Bee Demo](https://img.youtube.com/vi/w_m1j3X1hvY/0.jpg)](https://www.youtube.com/watch?v=w_m1j3X1hvY)

And that's about it. There were a lot of minor optimizations that I had to do this week regarding how to store references, minimizing iterations over collections, managing data lifetimes, and the like, which ate significantly into my time specifically regarding the hive implementation. I intended to get started with working on the structural aspects of the hive such as the worker bees, implementing brood/honey/structural comb, etc, but I simply did not get to it in time, so that will carry over into week 5, where I focus mostly on the structural aspects of the hive, and breeding, as well as refining the decision making process of the bees, and how the food source selection happens, rather than by simple random chance. More to come!

## Week 5 - Wanderlust

So week 5 was an interesting week for me. I saw some of my classmates utilizing perlin noise, and I saw some interesting organic movement simulations utilizing perlin noise flow fields, so I figured i would try my hand at it tos ee where I could get. This ended up being a very nice learning experience, but in the end I ditched it in favor or targeting random directions within a radius from the scout's current position.

The original implementation involved each scout bee generating a localized flow field around them; this flow field would add to their velocity in a given direction, from 0-2 radians based off the color of the pixel in that field. The problem with this was that it became increasingly non-performant as each scout would have to manually generate several thousand pixels of perlin noise whenever they left the local flow field. To optimize this, I created a flow field manager which would generate a few hundred flow fields, and disperse copies of them to bees as needed. This saved the cpu cost of generating the flow fields in real time, but the downside was that this still involved a significant amount of memory allocations, which I have yet to be able to address in a way that I completely like. That being said, it works, and for now I'm going to leave it until it becomes a problem.

Additionally, I implemented the weighted dice roll selection for onlooker bees. Now, whenever a scout returns to the hive, he performs a waggle dance, adding his food information to the hive, and then all onlookers that happen to be in the hive during hte waggle dance will look at the fitness of each of these food sources, currently only considering their food yield, and will choose slightly randomly, but heavily favored towards food sources that are more fit.

## Week 6 - Squad Goals 

Since we have the broad-strokes implementation of the ABC algorithm in terms of actually getting a communication system in place for scouting and foraging, the goal of this week is to flesh out those parts of the simulation that are not represented by the Artificial Bee Colony Algorithm. At a high-level perspective, these were my tasks for the week:

Improve movement/scouting  system
-	Give bees varied speed
-	Scouts pair with food source until it becomes unviable, then randomly scout again
-	Fix weighted selection bug for onlooker bees

Implement basic queen functionality
-	Determining when larva need to be spawned and laying them
-	Queen spawns larva based on two criteria:
	o	If the hive can support more bees, a larva will be spawned (ratio of total comb/bee population)
	o	Larva type is selected based on which type is needed most (each type has an optimal % of total population)

Implement basic larva
-	Spawns with a timer
-	Consumes hive resources until timer runs out
-	When timer runs out, destroys itself and spawns a bee

Implement basic drone features
-	Consumes resources to build structural comb
-	When a type of comb is needed, consumes energy to convert structural comb into honey comb or brood comb

Implement very basic soldier features
-	No enemies yet, so I can’t implement any way to attack
-	For now, the soldier will wander around the base, and they will be spawned if there aren’t enough soldiers

As you can see it seems like quite a lot compared to other weeks, but a lot of this work was simple foundational work; creating the base classes for different bee types, establishing their basic behhavior, and just get stuff moving on the screen.

For the improved movement, I received a criticism from my advisor that the bees tended to clump too much when they were persuing a food source; this made it hard to see what they were doing and it also simply didn't look organic. It was just too clear that they were all receiving simultaneous signals to perform tasks. The simple solution to this was to apply a slight randomness to the speed values of the bees. What this did was allow the bees to spread apart a bit when they received signals to travel to food sources, and really added to the organic feel of the simulation

The queen was relatively simple to implement. Since she really only does one thing, she didn't have a lot of states to consider when implementing her AI. She simply wanders around the hive, and when a type of larva is needed, she will lay one and it will hatch into th ebee type that it was specified on construction. For right now, she will just randomly decide what type of bee she wants to spawn, but once the comb structure is implemented, she will have a better idea of what ratio of bee types are needed so she can make more intelligent decisions as to what type of larva she lays

## Week 7 - Data Data Data

Week 7 is a week all about the data. Having the world generation be data driven is a very important goal for this project, as it will allow me the flexibility of not only being able to test very isolated circumstances without having to wait for simulations to run their course, but it will also look very interesting from a demonstration standpoint, and will allow for the user to generate worlds in whatever manner they see fit. In addition to that, I also took a large portion of the week to migrate the business logic of my simulation to a core library, so I could unit test and also open up the door to easily making the simulation mutli-platform down the road, if I so desire.

### JSON!
I had decided to use JSON just because I felt it offered a very good format for textually representing all of the data I wanted to have laid out in the world. This was fairly simple to implement; I added rapidjson to my project via NuGet, and simply created a manager to parse files and generate a world from them. An example world in my JSON grammar looks like the following:
```
{
	"Hives": 
	[
		{
			"position": {"x": 700, "y": 350},
			"Onlookers": 50,
			"Employees": 10,
			"Drones": 5,
			"Guards": 5
		}
	],
	"FoodSources":
	[
		{"position": {"x": -35, "y": -75}},
		{"position": {"x": 175, "y": -75}},
		{"position": {"x": 385, "y": -75}},
		{"position": {"x": 595, "y": -75}},
		{"position": {"x": 805, "y": -75}},
		{"position": {"x": 1015, "y": -75}},
		{"position": {"x": 1225, "y": -75}},
		{"position": {"x": 1435, "y": -75}},

		{"random_position": {"lower_bound": -10000.0 , "upper_bound": 10000.0}},
		{"random_position": {"lower_bound": -10000.0 , "upper_bound": 10000.0}},
		{"random_position": {"lower_bound": -10000.0 , "upper_bound": 10000.0}},
		{"random_position": {"lower_bound": -10000.0 , "upper_bound": 10000.0}}
	]
}
```

So you can see pretty clearly from the textual reprsentation that the hives are an array (denoted by []), each of which contains five data points: a position, and a number associated to each type of bee. I defaulted the number of queens to one per hive just for the sake of simplicity, but I should note that in the real world, hives can spawn additional queens when they're anticipating their own queen leaving the hive, or in response to the hive swarming, leaving some members behind.

Also it is worth noting that I added support for not only discrete positioning of hives and food sources, but also supporting random ranges for them as well. The last four food sources, for example, will spawn with a random x and a y between -10,000 and +10,000 respectively. This is also the specified size of the simulation area that I have specified, but both are arbitrary

### Speed!
Since I had differentiated bees based on speed, I thought it would be a nice touch to have time-dependent tasks be assocaited with their speed as well. So now, bees that are faster effectively are faster in every regard. Now, they will travel faster, metabolise faster (use up more energy per second), and they will accomplish tasks faster. This is important to note, becuase now not only will faster bees travel toward their destination faster, but they will also get a head start, which increases the spread and drastically improves the organic feel of the simulation

## Week 8 - Home is where the comb is

Here we are drastically approaching the minimum viable product for what I would consider a simulation. During this week, I focused primarily on getting comb structure implemented in the hive. For this, I needed to not only set up the infrastructure to track the amount of comb in the hive, but have the queen spawn larva based off of the amount of comb, and have drones actually work for a change. Now, the drones will take a look at how much comb is distributed across the hive, and if any comb is needed, they will construct or convert it accordingly. So, the general procedure for a drone is the following: Idle in the hive until comb is needed. If brood is needed to lay additional larva, convert some structural comb into brood comb. If more honey is in the hive than can adequately be stored, convert structural comb into honey comb. On top of that, whenever structural comb is consumed, construct more structural comb. It should be noted that constructing comb takes longer than simply appropriating it, so it could be the case that more drones are needed to build comb faster, which if that is the case, a request will be made to the queen to notify her that more drone larva are needed

### Optimizaiton!
Another aspect of the simulation I wanted to address during this week was implementing a collision grid. This would allow me to significantly reduce the collision checking that I'm doing in larger simulations. Currently, any entity checking for collisions with another entity will have to iterate over the entire list of entities, which can get pretty perofrmance draining. So I implemented what I called a "collision node" which is just a container which stores lists of entity pointers. Now, any entity registers itself with one collision node at a time, and they can check their own collision node as well as the 8 neighbors to it to perform collision calculations. These collision nodes are laid out in a grid that covers the entire simulation space, and even though it is not currently being used, it lays the foundation for drastic optimizations down the road

## Week 9 - The beginning of the end

This is the week where I try to get as much done as humanly possible, so I can finalize all of my work and prepare my presentation. The list of tasks I wanted to complete are as follows:

- Manage collision detections using collision grid
- Add metabolism and death conditions for bees
- Add Comb structure to hive
- Have workers build/convert comb as needed
- Have queen spawn most-needed larva, and only if the hive can support more

Hooking the collision detections up to the collision grid was suprisingly simple. Any time I wanted to perform collision checks, previously I would get the start iterator over the food source manager for example, and iterate over all food sources to check for collisions. Now, instead all I had to do was swap that iterator out for the start iterator of the collision node's food source vector. If there were no hits on the collision node, I would then expand my search to all 8 neighbors, breaking whenever I got a hit. After hooking these systems up to my collision grid system, I saw a factor of 10 performance increase on the collision checking system, which goes without saying, is a marked increase in performance.

Another large component to implement this week was a death condition. Bees had to start consuming energy. This was a fairly simple implementation, as all I needed to do was set an energy consumption rate, multiply that value by their speed percentage, and then the bees would consume energy from different sources depending on their state. For example, any time the bee is out of the hive, it will deplete energy from its own energy reserves, but if it is in a hive or a food source for any reason, it will consume energy from those storage sources rather than its own reserves. This added a very large layer of depth to the simulation, as now when running the simulation, the user can see the hive achieving homeostasis with the environment aorund it. If there are too many of one type of bee, they will start to die off and achieve a balance that fits the environment properly. Something that was a bit of a gotcha as far as managing death, was that all entities were stored in vectors in their respective manager singletons. For the savvy among you, you might know that if you delete an element in a vector, all iterators become invalidated. This became a problem if you were trying to destroy bees during the update loop. The solution to this was to simply add a mMarkedForDelete boolean in the entity class, so that way, after the update loop was completed, I could simply go through the vector and delete each value without having to worry about update calls being interrupted or iterators being invalidated. This required me to add a cleanup method for every entity type in all of my managers though, which is just an annoying bit of housekeeping I had to keep in consideration.

The final aspects I wanted to implement all occurred within the hive itself. I made it so that drones began building comb as needed, and queens would spawn larva based on that information. If there was enough comb to support more larva production, it would utilize as much of the brood comb as needed. Similarly, if there was enough food stored in the hive, it would increase the total yield of the hive, and lay however many larva were appropriate to meet the max hive size as well as maintaining the proper ratio of bee roles.

## Week 10 -

So this was the last week of work that I had scheduled for the semester, so my goal was to improve the visual aspect of the simulation as much as humanly possible. This included the following tasks:
 an intuitive HUD for hives to represent their internal features
	- Ratio of bee types represented in the hive
	- D
- Implementistribution of comb types in the hive (strutrual, brood, honey)
	- Representation of the amount of food in the hive
- Performance optimization as much as I possibly can
- Leveraging the data driven system to flesh out multiple worlds for my final presentation
- Prepare the demo, dry run and make no changes after that point
- Get wasps spawning and attacking as a proof of concept of hive predation

### Information!
So the HUD itself was fairly simple, as you can see in the following code, I essentially created a class that would hook into the class members of the Hive, and just represent them visually with a simple collection of sf::RectangleShape objects:

```
class HiveHUD
{
public:

	/**
	 *  Construtor
	 */
	HiveHUD(
		const sf::Vector2f& rootPosition, const sf::Vector2f& dimensions,
		const int& onlookerCount, const int& employeeCount, int& droneCount, const int& guardCount, const int& queenCount,
		const float& structuralComb, const float& honeyComb, const float& broodComb, const float& mFoodAmount);

	/**
	 * Destructor
	 */
	~HiveHUD() = default;

	// Copy / Move semantics
	HiveHUD(const HiveHUD& rhs) = delete;
	HiveHUD& operator=(const HiveHUD& rhs) = delete;
	HiveHUD(HiveHUD&& rhs) = delete;
	HiveHUD& operator=(HiveHUD&& rhs) = delete;

	/**
	 * Renders the HUD to the screen
	 * @Param window: The window that the HUD is being rendered to
	 */
	void Render(sf::RenderWindow& window) const;

	/**
	 *  Updates the relative size representations of the contents of the hive
	 */
	void UpdateHUDValues();

private:

	// Data references
	const int& mOnlookerCount;
	const int& mEmployeeCount;
	const int& mDroneCount;
	const int& mQueenCount;
	const int& mGuardCount;
	const float& mStructuralComb;
	const float& mHoneyComb;
	const float& mBroodComb;
	const float& mFoodAmount;

	// Rendering components
	sf::Vector2f mRootPosition;
	sf::Vector2f mDimensions;
	sf::RectangleShape mBeeContainer;
	sf::RectangleShape mCombContainer;
	sf::RectangleShape mFoodContainer;
	sf::RectangleShape mBarOnlookers;
	sf::RectangleShape mBarEmployees;
	sf::RectangleShape mBarDrones;
	sf::RectangleShape mBarGuards;
	sf::RectangleShape mBarQueens;
	sf::RectangleShape mBarStructuralComb;
	sf::RectangleShape mBarHoneyComb;
	sf::RectangleShape mBarBroodComb;
	sf::RectangleShape mBarFoodAmount;
	
	float mOutlineThickness;
};
```
### Power!
For the performance optimization step, I decided to finally give up on the perlin noise flow field implementation I was using for the wandering system. After doing a performance profile on the application, I noticed a significant amount of memory allocations were being made when new flow fields were being generated, and trying to scale the simulation up to handle a large number of hives and bees, this became increasingly impractical. I decided to switch the flow field generation out in favor of simply randomly choosing a point at a set distance away, and traveling to it. This turned out to look fine in the simulation, resulted in bees finding food sources more readily, and a significant performance increase, to the point where I could have 5 full size hives with 500+ bees running on start with no slowdown whatsoever, compared to the simulation lagging with one large hive with 50 scouts. This was a very good lesson in being prepared to kill your own baby, because I was very proud of my flow field system, and I still believe it could work effectively and look really nice in some implementation, it was simply way too performant to not include it, so the baby had to die this time.

### Data! 
So I had implemented the JSON parsing algorithm for world generation, and I needed to demonstrate quite a few things in a short period of time, so I thought that it would behoove me to leverage this system to benefit my demonstration.  My concern for presenting the simulation initially to people who haven't been as intimately familiar with the menutia as I have for the past several months, I wanted to ease them into understanding what they were seeing on the screen. For this, I wanted to introduce the roles of the bees in the presentation, then show a simple controlled environment with a very few number of bees to actually show how the interactions behave, and then progress to a large world where these interations would be massively up-scaled.
The benefit of demonstrating in this fashion, was that it also allowed me to demonstrate the data-driven aspect of the simulation, as the audience could see me loading drastically different worlds with simply changing of a command-line argument

### Bad Guys!
So the last thing I wanted to do, even though I had to limit my scope with adverse conditions, I wanted to implement a proof-of-concept predator that would at least introduce some conflict into the hive simulation. For this, I implemented a very basic wasp. He essentially has two states: wandering and attacking. he will behave exacxtly like the scout bee, randomly choosing locations to fly to, until he finds something he can interact with. In the case of the wasp, it looks for a hive. When a hive is encountered, it will begin randomly killing off bees until a guard bee intervenes. When a guard bee attacks the wasp, he kills the wasp as well as himself (for those of you that don't know, honey bees only have one good sting in them. When they sting they rip their stinger off and bleed to death). This implementation allowed me to demonstrate other features that were in place yet hard to notice, because the colonies do a very good job of maintaining homeostasis as it turns out. Now when bees die, the queen will be forced to prioritize larva that are needed, and she'll have to consume more energy to lay more larva, which in turn will influence the comb requirements, and all of a sudden there is just a lot more happening in the simulation, even though the infrastructure was already behaving "properly", the affects of that behavior were not completely apparent.

### Summary
All-in-all, I'm very happy with where I arrived with this project. It is the largest personal project of my own design that I have ever undertaken and I am immensely proud with where it has gone. I definitely intend to tinker with this over time and I'm curious just how far I can get this to go. I hope you all enjoyed it! Please feel free to fork and hack to your heart's content. I'll stay pretty active on this repo in the near future, so if you do something neat, feel free to throw a pull request my way, and I'll check it out!
