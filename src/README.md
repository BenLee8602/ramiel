# Code

## Camera
Stores the camera's current position and orientation. Contains functions for handling user input 
(moving camera), and converting 3D world coordinates to 3D camera coordinates to 2D screen 
coordinates.

## Collision
Functions for handling collision detection and response.

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
Post-processing effects. Defines the Effect type as a function that processes a rendered frame.

## Entity
An entity is a 3D object. It references a Model and Texture. Also contains shading type, physical 
properties, and a color if no texture is referenced.

## Graphics
Top level graphics module, stores all graphics related data, such as pixel buffers, entities, 
lights, and textures, and functions for graphics related operations like adding/removing entities, 
lighting, etc. from the scene, rendering frames, and more.

## Light
Defines classes for diffuse + specular lighting. This includes directional lighting (ex. the sun), 
point lighting, (ex. a light bulb) and spotlight (a flashlight). Each class implements a function 
to calculate the lighting on a given vertex.

## Mesh
Stores vertex geometry+texture, and polygon data for a 3D mesh.

## ObjReader
Functions for loading 3D geometry data from wavefront obj files.

## Physics
Top level physics module, stores all physics related data, such as physics objects and delta time, 
and function for physics related operations such as simulating dynamics and collisions.

## Physicsobj
Class that stores data for a physics object, such as mass, position, and rotation. Also contains 
derived classes for different collider types, like sphere, aabb, and mesh.

## Texture
Loads an image using stb_image and stores the texture in memory. Can return the color of the 
texture at a given normalized 2D coordinate. Also used for surface normal mapping.

## Triangle
Functions for clipping and rastering a triangle, based on the given Draw object and vertex data.

## Vec
Contains a class template, Vec<T, N> which contains a statically allocated array of type T and 
size N. These files also contain functions for vector math such as calculating dot products, cross 
products, and vector magnitude.
