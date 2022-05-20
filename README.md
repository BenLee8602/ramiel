# 3D Graphics Engine

This repos contains a 3D graphics + physics library. All graphics + physics related functionality 
was written without the use of any external libraries or frameworks.

![Mountains](https://github.com/BenLee8602/3D-Graphics-Engine/blob/master/screenshots/perlin_mountain.PNG?raw=true)
A mountain scene made using the following entities, lighting, and effects:
```cpp
GraphicsBL::loadModel("terrain", "examples/assets/models/terrain.obj", { -64, 0, -64 });
GraphicsBL::setAmbientLightColor({ 100, 80, 100 });
GraphicsBL::setBackgroundColor({ 150, 110, 110 });
GraphicsBL::addDirLight({ 155, 40, 0 }, { -10, 1, 0 });
GraphicsBL::addEntity("terrain", vec3f_255, ShadingType::PIXEL);
GraphicsBL::addEffect(new Fog(20, 100, { 150, 110, 110 }, true));
```

![Lighting Test](https://github.com/BenLee8602/3D-Graphics-Engine/blob/master/screenshots/cube.PNG?raw=true)
A test scene showing per-pixel lighting, spotlight, and a cube:
```cpp
GraphicsBL::loadModel("cube", "examples/assets/models/cube.obj");
GraphicsBL::setAmbientLightColor({ 25, 25, 25 });
GraphicsBL::addEntity("cube", vec3f_255, ShadingType::PIXEL);
GraphicsBL::addSpotLight(vec3f_255, { 0.8, 1, -2 }, { -0.25, -0.25, 1 });
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
correction, and texturing. Each class is responsible for clipping and interpolating vertex data 
across the triangle, drawing pixels, and executing any required setup for that drawing type.
These functions are called by a Triangle class instance.

## Effects
Defines classes for applying effects to a frame. Effects are applied to a frame after all the 
entities have been rendered. Some examples include blur, black and white, and fog.

## Entity
An entity is a 3D object. It references a Model and Texture. Also contains shading type, physical 
properties, and a color if no texture is referenced.

## GraphicsBL
Contains the GraphicsBL namespace. Public header contains the API, and the private header contains 
variables for things like entities, lighting, effects, models, textures, and more.

## Light
Defines classes for diffuse lighting. This includes directional lighting (ex. the sun), point 
lighting, (ex. a light bulb) and spotlight (a flashlight). Each class implements a function to 
calculate the lighting on a given vertex.

## Model
Stores vertex geometry+texture, and polygon data for a 3D model.

## ObjReader
Functions for loading 3D geometry data from wavefront obj files.

## Physics
Contains the physics properties of an entity, such as collisions and movement. It keeps track of 
things such as hitbox size, mass, velocity, and acceleration. It is also responsible for simulating 
entity movement and collisions for each frame.

## Texture
Loads an image using stb_image and stores the texture in memory. Can return the color of the 
texture at a given normalized 2D coordinate.

## Triangle
Functions for clipping and rastering a triangle, based on the given Draw object and vertex data.

## Vec
Contains a class template, Vec<T, N> which contains a statically allocated array of type T and 
size N. These files also contain functions for vector math such as calculating dot products, cross 
products, and vector magnitude.
