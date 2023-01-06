# Core

## Camera

```cpp
class Camera
```
Represents a camera in the 3D world
- `Vec3f pos` the camera's position
- `Rotation rot` the camera's rotation in radians
- `Vec3f backgroundColor` color buffer is filled with this color when resetBuffers is called
- `float znear` near z plane, used for clipping
- `float zfar` depth buffer is filled with this value when resetBuffers is called

- `Camera()` default constructor

- `size_t getBufferSize() const` returns size of color and depth buffers
- `const Vec2u& getRes() const` returns the camera's resolution
- `void setRes(Vec2u size)` sets the camera's resolution

- `float getFov() const` returns the camera's field of view
- `void setFov(float fov)` sets the camera's field of view

- `void resetBuffers()` fills the color and depth buffers with backgroundColor and zfar respectively

- `Vec3f getCameraCoord(const Vec3f& in) const` converts a 3D world coordinate to a 3D camera coordinate
- `Vec2f getScreenCoord(const Vec3f& in) const` converts a 3D camera coordinate to a 2D screen coordinate

- `ColorBufferIterator getColorBuffer()` returns an iterator to the beginning of the color buffer
- `DepthBufferIterator getDepthBuffer()` returns an iterator to the beginning of the depth buffer

- `void clampColorBuffer()` clamps the color buffer < 255

- `void getFrameDEC(int* frame) const` copies the color buffer into `frame`, in decimal format
- `void getFrameRGB(uint8_t* frame) const` copies the color buffer into `frame`, in rgb format

```cpp
class CameraModifier
```
Interface for a class that modifies a camera object in some way
- `virtual void run(Camera& camera) const = 0` the function that modifies a camera object

## Entity
```cpp
typedef CameraModifier EntityBase

template<class Vertex, class VertexShader, class PixelShader>
class Entity : public EntityBase
```
- `Entity(const Mesh<Vertex>& mesh, const VertexShader& vertexShader, const PixelShader& pixelShader)` constructs an entity using a mesh reference, a vertex shader, and a pixel shader
- `virtual void run(Camera& camera) const override` renders the entity using camera

## Light

```cpp
class Light
```
- `Light(Vec3f color, float intensity)` constructs Light
    - `Vec3f color` rgb color of the light
    - `float intensity` intensity/brightness of the light
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal) const = 0` calculates diffuse lighting based on the position and normal of the surface
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal, const Vec3f& cameraPos, unsigned specularExponent, float specularIntensity) const = 0` calculates diffuse + specular lighting based on the surface properties

```cpp
class DirectionalLight : public Light
```
- `DirectionalLight(Vec3f color, float intensity, Vec3f dir)` constructs DirectionalLight
    - `Vec3f color` rgb color of the light
    - `float intensity` intensity/brightness of the light
    - `Vec3f dir` a vector representing the lights direction
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal) const override` calculates diffuse lighting based on the position and normal of the surface
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal, const Vec3f& cameraPos, unsigned specularExponent, float specularIntensity) const override` calculates diffuse + specular lighting based on the surface properties

```cpp
class PointLight : public Light
```
- `PointLight(Vec3f color, float intensity, Vec3f pos, float falloff)` constructs PointLight
    - `Vec3f color` rgb color of the light
    - `float intensity` intensity/brightness of the light
    - `Vec3f pos` position of the light
    - `float falloff` light intensity falloff (linear)
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal) const override` calculates diffuse lighting based on the position and normal of the surface
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal, const Vec3f& cameraPos, unsigned specularExponent, float specularIntensity) const override` calculates diffuse + specular lighting based on the surface properties

```cpp
class SpotLight : public PointLight
```
- `SpotLight(Vec3f color, float intensity, Vec3f pos, Vec3f dir, float falloff, float width, float falloffExp)` construct SpotLight
    - `Vec3f color` rgb color of the light
    - `float intensity` intensity/brightness of the light
    - `Vec3f pos` position of the light
    - `Vec3f dir` a vector representing the lights direction
    - `float falloff` light intensity falloff (linear)
    - `float width` opening angle of the cone formed by the spotlight in radians
    - `float falloffExp` light intensity falloff as a surface position deviates from the spotlight's focus
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal) const override` calculates diffuse lighting based on the position and normal of the surface
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal, const Vec3f& cameraPos, unsigned specularExponent, float specularIntensity) const override` calculates diffuse + specular lighting based on the surface properties

```cpp
class LightingList
```
- `LightingList(const Vec3f& ambientLight, const std::vector<Light*>& lights)` constructs LightingList
    - `const Vec3f& ambientLight` ambient light color
    - `const std::vector<Light*>& lights` list of light sources
- `virtual Vec3f getAllLight(const Vec3f& pos, const Vec3f& normal) const` returns the sum of each light's effect on a surface
    - `const Vec3f& pos` position of the surface
    - `const Vec3f& normal` normal vector of the surface

```cpp
class LightingListSpecular : public LightingList
```
- `LightingListSpecular(const Vec3f& ambientLight, const std::vector<Light*>& lights, const Vec3f& cameraPos, uint16_t specularExponent, float specularIntensity)` constructs LightingListSpecular
    - `const Vec3f& ambientLight` ambient light color
    - `const std::vector<Light*>& lights` list of light sources
    - `const Vec3f& cameraPos` reference to a vector representing a camera's position
    - `uint16_t specularExponent` specular exponent of the surface this lightingList will be used for
    - `float specularIntensity` specular intensity of the surface this lightingList will be used for
- `virtual Vec3f getAllLight(const Vec3f& pos, const Vec3f& normal) const override` returns the sum of each light's effect on a surface
    - `const Vec3f& pos` position of the surface
    - `const Vec3f& normal` normal vector of the surface

## Mesh
```cpp
class MeshBase
```
- `const std::vector<Vec3u>& getTriangles() const` returns index buffer

```cpp
template<class Vertex>
class Mesh : public MeshBase
```
- `Mesh(const char* filename, bool loadvt = false, bool loadvn = false)` constructs Mesh from file
    - `const char* filename` path to a wavefront obj file to load from
    - `bool loadvt` tells the objloader to load texture coordinates or not
    - `bool loadvn` tells the objloader to load vertex normals or not
- `Mesh(std::vector<Vec3u>& triangles, std::vector<Vertex>& vertices)` constructs Mesh using an index and vertex buffer (copy)
- `Mesh(std::vector<Vec3u>&& triangles, std::vector<Vertex>&& vertices)` constructs Mesh using an index and vertex buffer (move)
- `const std::vector<Vertex>& getVertices() const` returns vertex buffer

## Rotation
```cpp
class Rotation
```
Wrapper around a Vec3f representing a rotation in 3 axis. Automatically calculates and stores sin and cos values when the rotation is changed.
- `Rotation()` default constructor, sets rotation to 0
- `Rotation(Vec3f rot)` constructs Rotation, sets rotation to `rot`

- `Rotation& operator=(const Vec3f& rot)` sets rotation to `rot` and calculates sin and cos
- `Rotation& operator+=(const Vec3f& rot)` adds `rot` to rotation and calculates sin and cos
- `void set(const Vec3f& rot)` sets rotation to `rot` and calculates sin and cos

- `float operator[](size_t i) const` returns the rotation for axis `i`
- `operator bool() const` returns false if rotation is 0 for x y and z
- `operator Vec3f() const` returns rotation
- `const Vec3f& get() const` returns rotation

- `const Vec3f& sin() const` returns sin(rotation)
- `const Vec3f& cos() const` returns cos(rotation)

- `Vec3f rotate(Vec3f in) const` returns `in`, rotated by the current rotation

## Scene
```cpp
class Scene
```
Encapsulates all resources and elements needed to simulate and render a scene
- `Camera camera` the camera used to render the scene
- `Vec3f ambientLight = vec3f_0` ambient light color

- `template<class Vertex> bool loadMesh(const char* filename, const char* meshname, bool loadvt = false, bool loadvn = false)` loads a mesh
    - `const char* filename` path to wavefront obj file to load from
    - `const char* meshname` name of the loaded mesh, used for retrieval
    - `bool loadvt` tells the objloader to load texture coordinates or not
    - `bool loadvn` tells the objloader to load vertex normals or not
- `bool loadTexture(const char* filename, const char* textureName)` loads a texture
    - `const char* filename` path to an image file to load
    - `const char* textureName` name of the loaded texture, used for retrieval
- `bool loadNormalMap(const char* filename, const char* normalMapName)` loads a normal map
    - `const char* filename` path to an image file to load
    - `const char* textureName` name of the loaded normal map, used for retrieval
- `const Texture* getTexture(const char* textureName) const` returns the texture named `textureName`

- `template<class Vertex, class VertexShader, class PixelShader> bool addEntity(const char* meshName, VertexShader vertexShader PixelShader pixelShader)` adds an entity to the scene
- `void addLight(Light* light)` adds a light source to the scene
- `void addEffect(Effect effect)` adds a post processing effect to the scene

- `LightingList getLightingList() const` returns a LightingList object for the scene
- `LightingListSpecular getLightingList(uint16_t specularExponent, float specularIntensity) const` returns a LightingListSpecular object for the scene

- `void renderFrame()` renders the scene. the rendered frame can be retrieved from the scene's camera

- `bool addPhysicsObject(PhysicsObject* physicsObject)` adds a physics object to the scene's physics object list
- `bool removePhysicsObject(PhysicsObject* physicsObject)` removes a physics object from the scene's physics object list

- `bool addCollider(Collider* collider)` adds a collider to the scene's collider list
- `bool removeCollider(Collider* collider)` removes a collider from the scene's collider list

- `void simulatePhysics(float dtime)` simulates physics object dynamics, and resolves collisions

## Texture
```cpp
class Texture
```
Stores a texture or normal map.
- `Texture(std::string filename, bool isNormalMap = false)` constructs Texture
    - `std::string filename` path to image file
    - `bool isNormalMap` false = load as RGB texture, true = load as normal map
- `Vec3f get(const Vec2f& coords) const` samples texture at given coordinates
- `Vec2 getSize() const` returns texture resolution
