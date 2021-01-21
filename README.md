![sponza1](/images/sponza1.png)
![sponza2](/images/sponza2.png)

sd
Aech is a game engine that I am developing in my spare time to gain a deeper understanding of everything involved in making a AAA video game.
The emphasis is on the 3D graphics subsystem as that is of my primary interest, however, the idea is to develop all the subsystems needed to create a modern video game, even though they won't be of high quality necessarily.

## Features 
As of right now, the following features are implemented:
* simple [Entity-Component-System architecture](https://en.wikipedia.org/wiki/Entity_component_system)
* material pipeline 
* 3D model loading
* PBR deferred rendering pipeline in OpenGL
* image-based lighting 
* normal mapping
* shadow mapping
* SSAO
* FXAA
* ACES and Reinhard tone mapping
* minimal GUI
* ...

TODO:
* refactor - most of the code was written a long time ago and in the meantime, I've learned better ways to do things
* port the renderer to Vulkan
* implement RTX features
