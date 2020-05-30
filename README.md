# Aech
Aech is a game engine that I am developing in my spare time to gain a deeper understanding of everything involved in making a AAA video game.
The emphasis is on the 3D graphics subsystem as that is of my primary interest, however, the idea is to develop all the subsystems needed to create a modern video game, even though they won't be of high quality necessarily.

## About the name 
The engine's architecture is based on the [Entity-Component-System architecture](https://en.wikipedia.org/wiki/Entity_component_system).
One of the popular ECS frameworks is called Artemis, which happens to be the name of a character from the movie [Ready Player One](https://www.imdb.com/title/tt1677720/), so I thought why not name my engine Aech which is also the name of a character from the same movie.

## Engine architecture
As I have already mentioned the architecture is based on the ECS architecture.
That means that the engine is composed of the following elements:
  * Entities - unique IDs used to identify game objects and hold components
  * Components - store data needed to implement particular behaviours
  * Systems - operate on entities which have the components required by the system and execute logic based on the data stored in those components

In the following subsections I will try to list all the systems and components that the engine is made of.

### Systems
![Aech systems](/diagrams/systems_diagram.png)

### TODO: Components
