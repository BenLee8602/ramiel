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

## Collider
```cpp
struct Collider : public Dynamics
```
Abstract class for a hitbox used for collision detection + response.
- `bool responsive` determines if this object responds to collisions
- `float mass` mass of the object
- `Collider(Dynamics dynamics = Dynamics(), bool responsive = true, float mass = 1.0f)` constructs collider

## Dynamics
```cpp
struct Dynamics : public Transform
```
Represents a dynamic physics object.
- `Vec3f posVel` velocity
- `Vec3f rotVel` rotational velocity
- `Vec3f posAcc` acceleration
- `Vec3f rotAcc` rotational acceleration
- `Dynamics(Vec3f pos = vec3f_0, Vec3f rot = vec3f_0, Vec3f posVel = vec3f_0, Vec3f rotVel = vec3f_0, Vec3f posAcc = vec3f_0, Vec3f rotAcc = vec3f_0)` construct Dynamics using `Vec3f`
- `Dynamics(Transform transform, Vec3f posVel = vec3f_0, Vec3f rotVel = vec3f_0, Vec3f posAcc = vec3f_0, Vec3f rotAcc = vec3f_0)` construct Dynamics using `Transform` + `Vec3f`

## Entity
```cpp
typedef CameraModifier EntityBase

template<class Vertex, class VertexShader, class PixelShader>
class Entity : public EntityBase
```
Represents a 3D object in the scene.
- `Entity(const Mesh<Vertex>& mesh, const VertexShader& vertexShader, const PixelShader& pixelShader)` constructs Entity
    - `const Mesh<Vertex>& mesh` reference to a mesh containing the 3D geometry for the entity to render
    - `const VertexShader& vertexShader` functor for transforming mesh vertices to another vertex type
    - `const PixelShader& pixelShader` class for transforming vertex attributes to an rgb color
- `virtual void run(Camera& camera) const override` renders the entity using `camera`

## Light

```cpp
class Light
```
Abstract class representing a light source.
- `Light(Vec3f color, float intensity)` constructs Light
    - `Vec3f color` rgb color of the light
    - `float intensity` intensity/brightness of the light
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal) const = 0` calculates diffuse lighting based on the position and normal of the surface
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal, const Vec3f& cameraPos, unsigned specularExponent, float specularIntensity) const = 0` calculates diffuse + specular lighting based on the surface properties

```cpp
class DirectionalLight : public Light
```
A global light source with parallel light "rays" (ex. the sun).
- `DirectionalLight(Vec3f color, float intensity, Vec3f dir)` constructs DirectionalLight
    - `Vec3f color` rgb color of the light
    - `float intensity` intensity/brightness of the light
    - `Vec3f dir` a vector representing the lights direction
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal) const override` calculates diffuse lighting based on the position and normal of the surface
- `virtual Vec3f getLight(const Vec3f& pos, const Vec3f& normal, const Vec3f& cameraPos, unsigned specularExponent, float specularIntensity) const override` calculates diffuse + specular lighting based on the surface properties

```cpp
class PointLight : public Light
```
A light source originating from a single point (ex. a lightbulb).
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
A light source originating from a single point and aimed at a specific direction (ex. a flashlight).
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
Contains a light of lights. Usually provided from `Scene` and used by shaders to calculate lighting.
- `LightingList(const Vec3f& ambientLight, const std::vector<Light*>& lights)` constructs LightingList
    - `const Vec3f& ambientLight` ambient light color
    - `const std::vector<Light*>& lights` list of light sources
- `virtual Vec3f getAllLight(const Vec3f& pos, const Vec3f& normal) const` returns the sum of each light's effect on a surface
    - `const Vec3f& pos` position of the surface
    - `const Vec3f& normal` normal vector of the surface

```cpp
class LightingListSpecular : public LightingList
```
Contains a light of lights, and specular reflection properties. Usually provided from `Scene` and used by shaders to calculate lighting.
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
A mesh stores 3D geometry data using a vertex buffer (storing position, texture coords, normals), and an index buffer (stores how vertices connect to form polygons). The mesh base class stores the index buffer.
- `const std::vector<Vec3u>& getTriangles() const` returns index buffer

```cpp
template<class Vertex>
class Mesh : public MeshBase
```
Stores a vertex buffer of type `Vertex`.
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

- `bool addDynamicObject(Dynamics* dynamicObject)` adds a dynamic object to the scene's dynamic object list
- `bool removeDynamicObject(Dynamics* dynamicObject)` removes a physics object from the scene's dynamic object list

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

## Transform
```cpp
struct Transform
```
Represents an affine tranformation
- `Vec3f pos` translation of the transformation
- `Rotation rot` rotation of the transformation
- `float scale` scaling factor of the transformation
- `Transform(Vec3f pos = vec3f_0, Rotation rot = vec3f_0, float scale = 1.0f)` constructs Transform
- `inline Vec3f operator()(const Vec3f& in) const` returns `in` after transformation

## Triangle
```cpp
template<typename Vertex, class PixelShader>
class Triangle
```
Used for clipping and rasterizing a triangle.
- `Triangle(Camera& camera, PixelShader pixelShader)`
    - `Camera& camera` the camera used to render the triangle
    - `PixelShader pixelShader` the pixel shader used to convert vertex attributes to a color
- `void draw(const Vertex& v0, const Vertex& v1, const Vertex& v2)` draws the triangle defined by `v1`, `v2`, and `v3`

## Vec
- `template<typename T, size_t N> std::ostream& operator<<(std::ostream& os, const Vec<T, N>& vec)` prints a vector

- `float getMagnitude(const Vec3f& v)` returns magnitude of a vector
- `Vec3f getNormalized(const Vec3f& v)` returns `v` normalized
- `Vec3f getNormalized(const Vec3f& v, float magnitude)` returns `v` normalized, skips magnitude calculation
- `float dotProduct(const Vec3f& vec1, const Vec3f& vec2)` returns the dot product of two vectors
- `Vec3f crossProduct(const Vec3f& vec1, const Vec3f& vec2)` returns the cross product of two vectors

- `void c_min(Vec3f& color, const Vec3f& min = vec3f_255)` clamps the upper bound of an rgb value
- `void c_max(Vec3f& color, const Vec3f& max = vec3f_0)` clamps the lower bound of an rgb value
- `void c_clamp(Vec3f& color)` clamps an rgb value between 0 and 255

```cpp
template<typename T, size_t N>
struct Vec
```
Vector of type `T` and size `N`.
- `T arr[N]` static array to store vector elements

- `T& operator[](size_t index)` element access
- `const T& operator[](size_t index) const` const element access

- `explicit operator T* ()` convert to `T` pointer
- `operator bool() const` boolean conversion, false if all elements are false
- `template<typename U> explicit operator Vec<U, N>() const` convert vector from type `T` to `U`

- `template<typename U> bool operator==(const Vec<U, N>& v) const` comparison, returns true if each element is equal
- `template<typename U> bool equals(const Vec<U, N>& v) const` comparison, with epsilon value for floating point

- `template<typename U> Vec<T, N>& operator=(const Vec<U, N>& v)` vector assignment, sets each element of `this` to `v`
- `template<typename U> Vec<T, N>& operator=(U num)` scalar assignment, sets each element of `this` to each element of `num`

- `template<typename U> Vec<T, N>& operator+=(U num)` scalar addition assignment, adds `num` to each element of `this`
- `template<typename U> Vec<T, N>& operator-=(U num)` scalar subtraction assignment, subtracts `num` from each element of `this`
- `template<typename U> Vec<T, N>& operator*=(U num)` scalar multiplication assignment, multiplies each element of `this` by `num`
- `template<typename U, typename F = float> Vec<T, N>& operator/=(U num)` scalar division assignment, divides each element of `this` by `num`

- `template<typename U> Vec<T, N>& operator+=(const Vec<U, N>& v)` vector addition assignment, sets `this` to the sum of the two vectors
- `template<typename U> Vec<T, N>& operator-=(const Vec<U, N>& v)` vector subtraction assignment, sets `this` to the difference of the two vectors
- `template<typename U> Vec<T, N>& operator*=(const Vec<U, N>& v)` vector multiplication assignment, multiplies each element of `this` by each element of `v`
- `template<typename U> Vec<T, N>& operator/=(const Vec<U, N>& v)` vector division assignment, divides each element of `this` by each element of `v`

- `template<typename U> Vec<T, N> operator+(U num) const` scalar addition, adds `num` to each element of `this`
- `template<typename U> Vec<T, N> operator-(U num) const` scalar subtraction subtracts `num` from each element of `this`
- `template<typename U> Vec<T, N> operator*(U num) const` scalar multiplication, multiplies each element of `this` by `num`
- `template<typename U, typename F = float> Vec<T, N> operator/(U num) const` scalar division, divides each element of `this` by `num`

- `template<typename U> Vec<T, N> operator+(const Vec<U, N>& v) const` vector addition, returns the sum of the two vectors
- `template<typename U> Vec<T, N> operator-(const Vec<U, N>& v) const` vector subtraction, returns the difference of the two vectors
- `template<typename U> Vec<T, N> operator*(const Vec<U, N>& v) const` vector multiplication, returns each element of `this` multiplied by each element of `v`
- `template<typename U> Vec<T, N> operator/(const Vec<U, N>& v) const` vector division, returns each element of `this` divided by each element of `v`

- `Vec<T, N> operator-() const` returns `this` negated
