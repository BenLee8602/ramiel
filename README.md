# ramiel

ramiel is a 3D graphics + physics library. All graphics + physics related functionality was 
written without the use of any external libraries or frameworks.

![Mountains](https://github.com/BenLee8602/ramiel/blob/master/screenshots/perlin_mountain.PNG?raw=true)
A mountain scene made using the following entities, lighting, and effects:
```cpp
graphics::loadModel("terrain", "examples/assets/models/terrain.obj", { -64, 0, -64 });
graphics::setAmbientLightColor({ 100, 80, 100 });
graphics::setBackgroundColor({ 150, 110, 110 });
graphics::addDirLight({ 155, 40, 0 }, { -10, 1, 0 });
graphics::addEntity("terrain", vec3f_255, nullptr, nullptr, ShadingType::PIXEL);
graphics::addEffect(new Fog(20, 100, { 150, 110, 110 }, true));
```

![Lighting Test](https://github.com/BenLee8602/ramiel/blob/master/screenshots/cube.PNG?raw=true)
A test scene showing per-pixel lighting, spotlight, and a cube:
```cpp
graphics::loadModel("cube", "examples/assets/models/cube.obj");
graphics::setAmbientLightColor({ 25, 25, 25 });
graphics::addEntity("cube", vec3f_255, nullptr, nullptr, ShadingType::PIXEL);
graphics::addSpotLight(vec3f_255, { 0.8, 1, -2 }, { -0.25, -0.25, 1 });
```

![Texture Mapping](https://github.com/BenLee8602/ramiel/blob/master/screenshots/cube_brick.png?raw=true)
A test scene showing a brick texture + normal map applied to cubes in various combinations.
Screenshot shows cube with both texture and normal map:
```cpp
graphics::loadTexture("brick_texture", "examples/assets/textures/brickwall_texture.jpg", 'c');
graphics::loadTexture("brick_normal", "examples/assets/textures/brickwall_normal.jpg", 'n');
graphics::loadModel("cube", "examples/assets/models/cube.obj");

// no texture or normal map
graphics::addEntity(
    "cube", { 100, 90, 70 }, nullptr, nullptr, ShadingType::PIXEL, { -1,  0,  1 },
    vec3f_0, false, 0, 0, true, vec3f_0, vec3f_0, { -0.1,  0.1,  0.1 }
);

// normal map only
graphics::addEntity(
    "cube", { 100, 90, 70 }, nullptr, "brick_normal", ShadingType::PIXEL, {  1,  0,  1 },
    vec3f_0, false, 0, 0, true, vec3f_0, vec3f_0, {  0.1,  0.1,  0.1 }
);

// texture only
graphics::addEntity(
    "cube", vec3f_255, "brick_texture", nullptr, ShadingType::PIXEL, { -1,  0, -1 },
    vec3f_0, false, 0, 0, true, vec3f_0, vec3f_0, { -0.1,  0.1, -0.1 }
);

// both texture and normal map
graphics::addEntity(
    "cube", vec3f_255, "brick_texture", "brick_normal", ShadingType::PIXEL, {  1,  0, -1 },
    vec3f_0, false, 0, 0, true, vec3f_0, vec3f_0, {  0.1,  0.1, -0.1 }
);

graphics::setAmbientLightColor({ 25, 25, 25 });
graphics::addPointLight(vec3f_255, { 0, 1, 0 }, 0.2);
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

## Ramiel
Contains the graphics namespace. Public header contains the API, and the private header contains 
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
texture at a given normalized 2D coordinate. Also used for surface normal mapping.

## Triangle
Functions for clipping and rastering a triangle, based on the given Draw object and vertex data.

## Vec
Contains a class template, Vec<T, N> which contains a statically allocated array of type T and 
size N. These files also contain functions for vector math such as calculating dot products, cross 
products, and vector magnitude.
