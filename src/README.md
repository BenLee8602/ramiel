# Code

## Camera
Represents a camera in the 3D world, including position, rotation, fov. Also contains color and 
depth buffers, and functions for transforming world coordinates to camera coordinates, and 
camera coordinates to screen coordinates.

## Collision
Functions for handling collision detection and response.

## Effects
Post-processing effects.

## Entity
An entity is a 3D object. It references a mesh, and contains vertex and pixel shaders which 
define how the mesh is rendered.

## Light
Light sources, including directional lights, point lights, and spotlights, and how to apply the 
light to a given surface.

## Mesh
Stores 3D geometry, using a vertex buffer, and an index buffer to assemble the vertices into 
triangles for rendering.

## Meshvertex
Vertex types to be stored in a mesh's vertex buffer, with attributes such as position/texture 
coordinates and surface normals.

## Objloader
Functions for loading 3D geometry data from wavefront obj files.

## Physicsobject
Contains physics properties of an object, such as position, velocity, mass, and behavior such as 
dynamics and collisions.

## Pixelshader
Example pixel shaders. Transforms an input vertex into an rgb color.

## Rotation
Stores a 3D rotation in radians, automatically calculates and stores sin and cos when the value 
is changed.

## Scene
Contains everything needed for simulating and rendering a 3D world, such as meshes, textures, 
entities, lights, effects, and physics objects.

## Texture
Loads an image from file using stb_image and stores it in memory. Can be used for rgb colors or 
normal vectors.

## Triangle
Functions for clipping and rasterizing a triangle represented by 3 vertices. Also takes a pixel 
shader to convert a vertex to rgb, and a camera for writing the rgb output to it's color buffer.

## Vec
Contains a class template, Vec<T, N> which contains a statically allocated array of type T and 
size N. These files also contain functions for vector math such as calculating dot products, cross 
products, and vector magnitude.

## Vertex
Example vertex types. Output of vertex shaders, used in rasterization and pixel shader input.

## Vertexshader
Example vertex shaders. Transforms an input vertex into an output vertex.
