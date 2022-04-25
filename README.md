# 3D Graphics Engine

This repos contains a 3D graphics engine, written from scratch, without the use of any graphics 
libraries. Currently only runs on CPU.

![Mountains](https://github.com/BenLee8602/3D-Graphics-Engine/blob/master/screenshots/perlin_mountain.PNG?raw=true)
A mountain scene made using the following entities, lighting, and effects:
```cpp
GraphicsBL::setAmbientLightColor({ 100, 80, 100 });
GraphicsBL::setBackgroundColor({ 150, 110, 110 });
GraphicsBL::addLight(new Light_Dir({ 155, 40, 0 }, { -10, 1, 0 }));
GraphicsBL::addEntity(new Entity(
	"model/terrain3.obj",
	ShadingType::FLAT,
	vec3f_255,
	Physics(Vec3f{ -64, 0, -64 })
));
GraphicsBL::addEffect(new Fog(20, 100, { 150, 110, 110 }, true));
GraphicsBL::addEffect(new Bloom(5, false));
```

![Lighting Test](https://github.com/BenLee8602/3D-Graphics-Engine/blob/master/screenshots/cube.PNG?raw=true)
A test scene showing per-pixel lighting, spotlight, and a cube:
```cpp
GraphicsBL::setAmbientLightColor({ 25, 25, 25 });
GraphicsBL::addEntity(new Entity("model/cube.obj", ShadingType::PIXEL));
GraphicsBL::addLight(new Light_Sp(vec3f_255, { 0.8, 1, -2 }, { -0.25, -0.25, 1 }));	
```


# Code

## Camera
Stores the camera's current position and orientation. Contains functions for handling user input, 
and converting 3D world coordinates to 3D camera coordinates to 2D screen coordinates.

## Draw
Contains the lowest level code, involved directly with drawing pixels to the pixel buffer. Defines 
a class for each drawing type:
* Flat: one color is used for drawing the entire triangle
* Vertex: colors from each vertex are interpolated across the triangle face, giving a smoother look
* Pixel: colors are calculated for each pixel, giving the most realistic look

There are also templated draw classes for modifying the main 3 ^, for things like perspective 
correction, and texturing (WIP). Each class is responsible for clipping and interpolating vertex 
data across the triangle, drawing pixels, and executing any required setup for that drawing type.
These functions are called by a Triangle class instance.

## Effects
Defines classes for applying effects to a frame. Effects are applied to a frame after all the 
entities have been rendered. Some examples include blur, black and white, and fog.

## Entity
An entity is a 3D object. It contains arrays for vertices and triangles. It also defines the 
object's shading type, physics properties, and color.

## Graphics
The main graphics class, contains things like window dimensions, pixel buffers, entities, lighting, 
and effects. Also contains functions for modifying these fields, and rendering a frame.

## Light
Defines classes for lighting. This includes directional lighting (ex. the sun), point lighting, 
(ex. a light bulb) and spotlight (a flashlight). Each class implements a function to calculate the 
lighting on a given vertex.

## Main
Example client code: uses windows.h to open a window, adds entities, lights, effects to the scene, 
and contains the main loop. This main loop keeps track of user input, time elapsed between frames, 
then renders the frame and displays it in the window.

## Physics
Contains the physics properties of an entity, such as collisions and movement. It keeps track of 
things such as hitbox size, mass, velocity, and acceleration. It is also responsible for simulating 
entity movement and collisions for each frame.

## Triangle
The Triangle class defines a triangle using 3 integers. These integers represent indices in the 
vertices array in the Entity class. It also contains functions for clipping and rastering a 
triangle.

## Vec
Contains a class template, Vec<T, N> which contains a statically allocated array of type T and 
size N. These files also contain functions for vector math such as calculating dot products, cross 
products, and vector magnitude.
