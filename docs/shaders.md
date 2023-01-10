# Shaders

## Vertex Types
```cpp
class Vertex_PerTri
```
Vertex type for rendering an entity with lighting calculated per-triangle.
- `Vec3f worldPos` world-space position
- `Vec3f cameraPos` camera-space position
- `Vec2f screenPos` screen-space position
- `Vertex_PerTri operator+(const Vertex_PerTri& other) const` adds all vertex attributes
- `Vertex_PerTri operator-(const Vertex_PerTri& other) const` subtracts all vertex attributes
- `Vertex_PerTri operator*(float n) const` multiplies all vertex attributes by `n`
- `Vertex_PerTri operator/(float n) const` divides all vertex attributes by `n`
- `Vertex_PerTri& operator+=(const Vertex_PerTri& other)` adds vertex attributes from `other` to `this`

```cpp
class Vertex_PerTri_Textured
```
Vertex type for rendering an entity with lighting calculated per-triangle, textured.
- `Vec3f worldPos` world-space position
- `Vec3f cameraPos` camera-space position
- `Vec2f screenPos` screen-space position
- `float zinv` inverse of camera-space z, for perspective correction
- `Vec2f texturePos` texture-space position
- `Vertex_PerTri_Textured operator+(const Vertex_PerTri_Textured& other) const` adds all vertex attributes
- `Vertex_PerTri_Textured operator-(const Vertex_PerTri_Textured& other) const` subtracts all vertex attributes
- `Vertex_PerTri_Textured operator*(float n) const` multiplies all vertex attributes by `n`
- `Vertex_PerTri_Textured operator/(float n) const` divides all vertex attributes by `n`
- `Vertex_PerTri_Textured& operator+=(const Vertex_PerTri_Textured& other)` adds vertex attributes from `other` to `this`

```cpp
class Vertex_PerVertex
```
Vertex type for rendering an entity with lighting calculated per-vertex.
- `Vec3f cameraPos` camera-space position
- `Vec2f screenPos` screen-space position
- `Vec3f color` rgb color of the vertex
- `Vertex_PerVertex operator+(const Vertex_PerVertex& other) const` adds all vertex attributes
- `Vertex_PerVertex operator-(const Vertex_PerVertex& other) const` subtracts all vertex attributes
- `Vertex_PerVertex operator*(float n) const` multiplies all vertex attributes by `n`
- `Vertex_PerVertex operator/(float n) const` divides all vertex attributes by `n`
- `Vertex_PerVertex& operator+=(const Vertex_PerVertex& other)` adds vertex attributes from `other` to `this`

```cpp
class Vertex_PerVertex_Textured
```
Vertex type for rendering an entity with lighting calculated per-vertex, textured.
- `Vec3f cameraPos` camera-space position
- `Vec2f screenPos` screen-space position
- `Vec3f light` sum of all light that affects the vertex
- `float zinv` inverse of camera-space z, for perspective correction
- `Vec2f texturePos` texture-space position
- `Vertex_PerVertex_Textured operator+(const Vertex_PerVertex_Textured& other) const` adds all vertex attributes
- `Vertex_PerVertex_Textured operator-(const Vertex_PerVertex_Textured& other) const` subtracts all vertex attributes
- `Vertex_PerVertex_Textured operator*(float n) const` multiplies all vertex attributes by `n`
- `Vertex_PerVertex_Textured operator/(float n) const` divides all vertex attributes by `n`
- `Vertex_PerVertex_Textured& operator+=(const Vertex_PerVertex_Textured& other)` adds vertex attributes from `other` to `this`

```cpp
class Vertex_PerPixel
```
Vertex type for rendering an entity with lighting calculated per-pixel.
- `Vec3f worldPos` world-space position
- `Vec3f cameraPos` camera-space position
- `Vec2f screenPos` screen-space position
- `float zinv` inverse of camera-space z, for perspective correction
- `Vertex_PerPixel operator+(const Vertex_PerPixel& other) const` adds all vertex attributes
- `Vertex_PerPixel operator-(const Vertex_PerPixel& other) const` subtracts all vertex attributes
- `Vertex_PerPixel operator*(float n) const` multiplies all vertex attributes by `n`
- `Vertex_PerPixel operator/(float n) const` divides all vertex attributes by `n`
- `Vertex_PerPixel& operator+=(const Vertex_PerPixel& other)` adds vertex attributes from `other` to `this`

```cpp
class Vertex_PerPixel_Textured
```
Vertex type for rendering an entity with lighting calculated per-pixel, textured.
- `Vec3f worldPos` world-space position
- `Vec3f cameraPos` camera-space position
- `Vec2f screenPos` screen-space position
- `float zinv` inverse of camera-space z, for perspective correction
- `Vec2f texturePos` texture-space position
- `Vertex_PerPixel_Textured operator+(const Vertex_PerPixel_Textured& other) const` adds all vertex attributes
- `Vertex_PerPixel_Textured operator-(const Vertex_PerPixel_Textured& other) const` subtracts all vertex attributes
- `Vertex_PerPixel_Textured operator*(float n) const` multiplies all vertex attributes by `n`
- `Vertex_PerPixel_Textured operator/(float n) const` divides all vertex attributes by `n`
- `Vertex_PerPixel_Textured& operator+=(const Vertex_PerPixel_Textured& other)` adds vertex attributes from `other` to `this`

```cpp
class Vertex_PerPixel_Smooth
```
Vertex type for rendering an entity with lighting calculated per-pixel, with a smooth appearance.
- `Vec3f worldPos` world-space position
- `Vec3f cameraPos` camera-space position
- `Vec2f screenPos` screen-space position
- `Vec3f normal` vertex normal vector
- `float zinv` inverse of camera-space z, for perspective correction
- `Vertex_PerPixel_Smooth operator+(const Vertex_PerPixel_Smooth& other) const` adds all vertex attributes
- `Vertex_PerPixel_Smooth operator-(const Vertex_PerPixel_Smooth& other) const` subtracts all vertex attributes
- `Vertex_PerPixel_Smooth operator*(float n) const` multiplies all vertex attributes by `n`
- `Vertex_PerPixel_Smooth operator/(float n) const` divides all vertex attributes by `n`
- `Vertex_PerPixel_Smooth& operator+=(const Vertex_PerPixel_Smooth& other)` adds vertex attributes from `other` to `this`

```cpp
class Vertex_PerPixel_Smooth_Textured
```
Vertex type for rendering an entity with lighting calculated per-pixel, with a smooth appearance, and textured.
- `Vec3f worldPos` world-space position
- `Vec3f cameraPos` camera-space position
- `Vec2f screenPos` screen-space position
- `Vec3f normal` vertex normal vector
- `float zinv` inverse of camera-space z, for perspective correction
- `Vec2f texturePos` texture-space position
- `Vertex_PerPixel_Smooth_Textured operator+(const Vertex_PerPixel_Smooth_Textured& other) const` adds all vertex attributes
- `Vertex_PerPixel_Smooth_Textured operator-(const Vertex_PerPixel_Smooth_Textured& other) const` subtracts all vertex attributes
- `Vertex_PerPixel_Smooth_Textured operator*(float n) const` multiplies all vertex attributes by `n`
- `Vertex_PerPixel_Smooth_Textured operator/(float n) const` divides all vertex attributes by `n`
- `Vertex_PerPixel_Smooth_Textured& operator+=(const Vertex_PerPixel_Smooth_Textured& other)` adds vertex attributes from `other` to `this`

## Vertex Shaders
```cpp
class VS_PerTri
```
Vertex shader for rendering an entity with lighting calculated per-triangle.
- `VS_PerTri(Camera& camera, Transform* transform)` constructor
    - `Camera& camera` camera used for camera and screen transformations
    - `Transform* transform` object-space to world-space transformation
- `typedef Vertex_PerTri Vertex_Out` this vertex shader outputs a `Vertex_PerTri`
- `template<class Vertex_In> Vertex_Out operator()(const Vertex_In& in) const` transforms a `Vertex_In` into a `Vertex_Out`

```cpp
class VS_PerTri_Textured
```
Vertex shader for rendering an entity with lighting calculated per-triangle, textured.
- `VS_PerTri_Textured(Camera& camera, Transform* transform)` constructor
    - `Camera& camera` camera used for camera and screen transformations
    - `Transform* transform` object-space to world-space transformation
- `typedef Vertex_PerTri_Textured Vertex_Out` this vertex shader outputs a `Vertex_PerTri_Textured`
- `template<class Vertex_In> Vertex_Out operator()(const Vertex_In& in) const` transforms a `Vertex_In` into a `Vertex_Out`

```cpp
class VS_PerVertex
```
Vertex shader for rendering an entity with lighting calculated per-vertex.
- `VS_PerVertex(Camera& camera, Transform* transform, LightingListSpecular lightingList, Vec3f surfaceColor = vec3f_255)` constructor
    - `Camera& camera` camera used for camera and screen transformations
    - `Transform* transform` object-space to world-space transformation
    - `LightingListSpecular lightingList` list of light sources for per-vertex lighting calculations
    - `Vec3f surfaceColor` surface color for calculating vertex colors
- `typedef Vertex_PerVertex Vertex_Out` this vertex shader outputs a `Vertex_PerVertex`
- `template<class Vertex_In> Vertex_Out operator()(const Vertex_In& in) const` transforms a `Vertex_In` into a `Vertex_Out`

```cpp
class VS_PerVertex_Textured
```
Vertex shader for rendering an entity with lighting calculated per-vertex, textured.
- `VS_PerVertex_Textured(Camera& camera, Transform* transform, LightingListSpecular lightingList)` constructor
    - `Camera& camera` camera used for camera and screen transformations
    - `Transform* transform` object-space to world-space transformation
    - `LightingListSpecular lightingList` list of light sources for per-vertex lighting calculations
- `typedef Vertex_PerVertex_Textured Vertex_Out` this vertex shader outputs a `Vertex_PerVertex_Textured`
- `template<class Vertex_In> Vertex_Out operator()(const Vertex_In& in) const` transforms a `Vertex_In` into a `Vertex_Out`

```cpp
class VS_PerPixel
```
Vertex shader for rendering an entity with lighting calculated per-pixel.
- `VS_PerPixel(Camera& camera, Transform* transform)` constructor
    - `Camera& camera` camera used for camera and screen transformations
    - `Transform* transform` object-space to world-space transformation
- `typedef Vertex_PerPixel Vertex_Out` this vertex shader outputs a `Vertex_PerPixel`
- `template<class Vertex_In> Vertex_Out operator()(const Vertex_In& in) const` transforms a `Vertex_In` into a `Vertex_Out`

```cpp
class VS_PerPixel_Textured
```
Vertex shader for rendering an entity with lighting calculated per-pixel, textured.
- `VS_PerPixel_Textured(Camera& camera, Transform* transform)` constructor
    - `Camera& camera` camera used for camera and screen transformations
    - `Transform* transform` object-space to world-space transformation
- `typedef VS_PerPixel_Textured Vertex_Out` this vertex shader outputs a `VS_PerPixel_Textured`
- `template<class Vertex_In> Vertex_Out operator()(const Vertex_In& in) const` transforms a `Vertex_In` into a `Vertex_Out`

```cpp
class VS_PerPixel_Smooth
```
Vertex shader for rendering an entity with lighting calculated per-pixel, with a smooth appearance.
- `VS_PerPixel_Smooth(Camera& camera, Transform* transform)` constructor
    - `Camera& camera` camera used for camera and screen transformations
    - `Transform* transform` object-space to world-space transformation
- `typedef VS_PerPixel_Smooth Vertex_Out` this vertex shader outputs a `VS_PerPixel_Smooth`
- `template<class Vertex_In> Vertex_Out operator()(const Vertex_In& in) const` transforms a `Vertex_In` into a `Vertex_Out`

```cpp
class VS_PerPixel_Smooth_Textured
```
Vertex shader for rendering an entity with lighting calculated per-pixel, with a smooth appearance, and textured.
- `VS_PerPixel_Smooth_Textured(Camera& camera, Transform* transform)` constructor
    - `Camera& camera` camera used for camera and screen transformations
    - `Transform* transform` object-space to world-space transformation
- `typedef VS_PerPixel_Smooth_Textured Vertex_Out` this vertex shader outputs a `VS_PerPixel_Smooth_Textured`
- `template<class Vertex_In> Vertex_Out operator()(const Vertex_In& in) const` transforms a `Vertex_In` into a `Vertex_Out`

## Pixel Shaders
```cpp
class PS_PerTri
```
Pixel shader for rendering an entity with lighting calculated per-triangle.
- `PS_PerTri(LightingListSpecular lightingList, Vec3f surfaceColor)` constructor
    - `LightingListSpecular lightingList` list of light sources for per-triangle lighting calculations
    - `Vec3f surfaceColor` surface color for triangle color calculations
- `template<class Vertex_In> void init(Vertex_In v[3])` performs per-triangle calculations using the triangle defined by `v`
- `template<class Vertex_In> Vec3f draw(const Vertex_In& v)` converts vertex attributes into an rgb color

```cpp
class PS_PerTri_Textured
```
Pixel shader for rendering an entity with lighting calculated per-triangle, textured.
- `PS_PerTri_Textured(LightingListSpecular lightingList, const Texture* texture)` constructor
    - `LightingListSpecular lightingList` list of light sources for per-triangle lighting calculations
    - `const Texture* texture` texture sampled for surface colors
- `template<class Vertex_In> void init(Vertex_In v[3])` performs per-triangle calculations using the triangle defined by `v`
- `template<class Vertex_In> Vec3f draw(const Vertex_In& v)` converts vertex attributes into an rgb color

```cpp
class PS_PerVertex
```
Pixel shader for rendering an entity with lighting calculated per-vertex.
- `template<class Vertex_In> void init(Vertex_In v[3])` performs per-triangle calculations using the triangle defined by `v`
- `template<class Vertex_In> Vec3f draw(const Vertex_In& v)` converts vertex attributes into an rgb color

```cpp
class PS_PerVertex_Textured
```
Pixel shader for rendering an entity with lighting calculated per-vertex, textured.
- `PS_PerVertex_Textured(const Texture* texture)` constructor
    - `const Texture* texture` texture sampled for surface colors
- `template<class Vertex_In> void init(Vertex_In v[3])` performs per-triangle calculations using the triangle defined by `v`
- `template<class Vertex_In> Vec3f draw(const Vertex_In& v)` converts vertex attributes into an rgb color

```cpp
class PS_PerPixel
```
Pixel shader for rendering an entity with lighting calculated per-pixel.
- `PS_PerPixel(LightingListSpecular lightingList, Vec3f surfaceColor)` constructor
    - `LightingListSpecular lightingList` list of light sources for per-pixel lighting calculations
    - `Vec3f surfaceColor` surface color for pixel color calculations
- `template<class Vertex_In> void init(Vertex_In v[3])` performs per-triangle calculations using the triangle defined by `v`
- `template<class Vertex_In> Vec3f draw(const Vertex_In& v)` converts vertex attributes into an rgb color

```cpp
class PS_PerPixel_Textured
```
Pixel shader for rendering an entity with lighting calculated per-pixel, textured.
- `PS_PerPixel_Textured(LightingListSpecular lightingList, const Texture* texture)` constructor
    - `LightingListSpecular lightingList` list of light sources for per-pixel lighting calculations
    - `const Texture* texture` texture sampled for surface colors
- `template<class Vertex_In> void init(Vertex_In v[3])` performs per-triangle calculations using the triangle defined by `v`
- `template<class Vertex_In> Vec3f draw(const Vertex_In& v)` converts vertex attributes into an rgb color

```cpp
class PS_PerPixel_NormalMapped
```
Pixel shader for rendering an entity with lighting calculated per-pixel, using a normal map.
- `PS_PerPixel_NormalMapped(LightingListSpecular lightingList, const Texture* normalMap, Vec3f surfaceColor)` constructor
    - `LightingListSpecular lightingList` list of light sources for per-pixel lighting calculations
    - `const Texture* normalMap` normal map sampled for surface normals
    - `Vec3f surfaceColor` surface color for pixel color calculations
- `template<class Vertex_In> void init(Vertex_In v[3])` performs per-triangle calculations using the triangle defined by `v`
- `template<class Vertex_In> Vec3f draw(const Vertex_In& v)` converts vertex attributes into an rgb color

```cpp
class PS_PerPixel_Textured_NormalMapped
```
Pixel shader for rendering an entity with lighting calculated per-pixel, using a texture and normal map.
- `PS_PerPixel_Textured_NormalMapped(LightingListSpecular lightingList, const Texture* texture, const Texture* normalMap)` constructor
    - `LightingListSpecular lightingList` list of light sources for per-pixel lighting calculations
    - `const Texture* texture` texture sampled for surface colors
    - `const Texture* normalMap` normal map sampled for surface normals
- `template<class Vertex_In> void init(Vertex_In v[3])` performs per-triangle calculations using the triangle defined by `v`
- `template<class Vertex_In> Vec3f draw(const Vertex_In& v)` converts vertex attributes into an rgb color

```cpp
class PS_PerPixel_Smooth
```
Pixel shader for rendering an entity with lighting calculated per-pixel, with a smooth appearance.
- `PS_PerPixel_Smooth(LightingListSpecular lightingList, Vec3f surfaceColor)` constructor
    - `LightingListSpecular lightingList` list of light sources for per-pixel lighting calculations
    - `Vec3f surfaceColor` surface color for pixel color calculations
- `template<class Vertex_In> void init(Vertex_In v[3])` performs per-triangle calculations using the triangle defined by `v`
- `template<class Vertex_In> Vec3f draw(const Vertex_In& v)` converts vertex attributes into an rgb color

```cpp
class PS_PerPixel_Smooth_Textured
```
Pixel shader for rendering an entity with lighting calculated per-pixel, with a smooth appearance, and textured.
- `PS_PerPixel_Smooth_Textured(LightingListSpecular lightingList, const Texture* texture)` constructor
    - `LightingListSpecular lightingList` list of light sources for per-pixel lighting calculations
    - `const Texture* texture` texture sampled for surface colors
- `template<class Vertex_In> void init(Vertex_In v[3])` performs per-triangle calculations using the triangle defined by `v`
- `template<class Vertex_In> Vec3f draw(const Vertex_In& v)` converts vertex attributes into an rgb color
