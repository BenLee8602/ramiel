# Effects

```cpp
typedef CameraModifier Effect;
```

**If you choose to use these effects, they should be applied in the following order:**
1. Fog
2. Exposure
3. Contrast
4. Brightness
5. Color filtering
6. Saturation
7. Tone mapping

```cpp
class Brightness : public Effect
```
Adjust the brightness of a camera's color buffer.
- `Brightness(float brightness)` constructor, negative values for `brightness` decrease brightness, positive increases.
- `virtual void run(Camera& camera) const override` applies effect to `camera`

```cpp
class ColorFilter : public Effect
```
Filter colors from a camera's color buffer.
- `ColorFilter(Vec3f color)` constructor, `color` is the rgb color used for filtering.
- `virtual void run(Camera& camera) const override` applies effect to `camera`

```cpp
class Contrast : public Effect
```
Adjust the contrast of a camera's color buffer.
- `Contrast(float contrast)` constructor, `0 < contrast < 1` decreases contrast, `contrast > 1` increases.
- `virtual void run(Camera& camera) const override` applies effect to `camera`

```cpp
class Exposure : public Effect
```
Adjust the exposure of a camera's color buffer.
- `Exposure(float exposure)` constructor, `0 < exposure < 1` decreases exposure, `exposure > 1` increases.
- `virtual void run(Camera& camera) const override` applies effect to `camera`

```cpp
class Fog : public Effect
```
Apply fog to a camera's color buffer.
- `Fog(Vec3f fogColor, float fogStart, float fogEnd)` constructor
    - `Vec3f fogColor` color of the fog
    - `float fogStart` distance where fog begins to affect pixel colors
    - `float fogEnd` distance where fog completely 
- `virtual void run(Camera& camera) const override` applies effect to `camera`

```cpp
class ToneMapping : public Effect
```
Convert a camera's color buffer from high dynamic range to low dynamic range.
- `virtual void run(Camera& camera) const override` applies effect to `camera`

```cpp
class Saturation : public Effect
```
Adjust the saturation of a camera's color buffer
- `Saturation(float saturation)` constructor, `0 < saturation < 1` decreases saturation, `saturation > 1` increases.
- `virtual void run(Camera& camera) const override` applies effect to `camera`
