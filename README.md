# ramiel

ramiel is a 3D graphics + physics library. All graphics + physics related functionality was 
written without the use of any external libraries or frameworks.

![Mountains](https://github.com/BenLee8602/ramiel/blob/master/screenshots/perlin_mountain.PNG?raw=true)
A mountain scene made using the following entities, lighting, and effects:
```cpp
graphics::loadMesh("terrain", "examples/assets/models/terrain.obj", 1.0f, { -64, 0, -64 });
graphics::setAmbientLightColor({ 100, 80, 100 });
graphics::setBackgroundColor({ 150, 110, 110 });
graphics::addDirLight({ 155, 40, 0 }, 1.0f, { -10, 1, 0 });
graphics::addEntity("terrain");
graphics::addEffect(fog<150, 110, 110, 20, 100>);
```

![Lighting Test](https://github.com/BenLee8602/ramiel/blob/master/screenshots/ramiel.PNG?raw=true)
A test scene showing per-pixel lighting, a spotlight, specular reflection, and an octahedron:
```cpp
graphics::loadMesh("ramiel", "examples/assets/models/ramiel.obj");
graphics::setAmbientLightColor({ 50, 50, 50 });
graphics::addEntity({
    { "mesh", "ramiel" },
    { "pos", Vec3f{ 0, 0, 4 } },
    { "color", Vec3f{ 70, 135, 255 } },
    { "shading", ShadingType::PIXEL },
    { "specularExponent", 256U },
    { "specularIntensity", 1.0f }
});
graphics::addSpotLight(vec3f_255, 4.0f, { -1, 2, 3 }, { 1, -2, 1 });
```

![Texture Mapping](https://github.com/BenLee8602/ramiel/blob/master/screenshots/brick.PNG?raw=true)
A test scene showing 4 cubes, with textures and normal mapping.
* Top: no texture or normal map
* Left: textured only
* Right: normal mapped only
* Bottom: textured and normal mapped
```cpp
graphics::loadTexture("brick_texture", "examples/assets/textures/brickwall_texture.jpg", 'c');
graphics::loadTexture("brick_normal", "examples/assets/textures/brickwall_normal.jpg", 'n');
graphics::loadMesh("cube", "examples/assets/models/cube.obj");

// lighting
graphics::setAmbientLightColor({ 25, 25, 25 });
graphics::addPointLight(vec3f_255, 4.0f, { 0, 1, 0 }, 0.2);
graphics::addEffect(hdr);

// no texture or normal map
graphics::addEntity({
    { "mesh", "cube" },
    { "specularExponent", 4U },
    { "specularIntensity", 0.1f },
    { "shading", ShadingType::PIXEL },
    { "dynamic", true },
    { "pos", Vec3f{ -1, 0, 1 } },
    { "rotVel", Vec3f{ -0.1,  0.1,  0.1 } },
    { "color", Vec3f{ 100, 90, 70 } }
});

// normal map only
graphics::addEntity({
    { "mesh", "cube" },
    { "specularExponent", 4U },
    { "specularIntensity", 0.1f },
    { "shading", ShadingType::PIXEL },
    { "dynamic", true },
    { "pos", Vec3f{ 1, 0, 1 } },
    { "rotVel", Vec3f{ 0.1, 0.1, 0.1 } },
    { "color", Vec3f{ 100, 90, 70 } },
    { "normalMap", "brick_normal" }
});

// texture only
graphics::addEntity({
    { "mesh", "cube" },
    { "specularExponent", 4U },
    { "specularIntensity", 0.1f },
    { "shading", ShadingType::PIXEL },
    { "dynamic", true },
    { "pos", Vec3f{ -1, 0, -1 } },
    { "rotVel", Vec3f{ -0.1, 0.1, -0.1 } },
    { "texture", "brick_texture" }
});

// both texture and normal map
graphics::addEntity({
    { "mesh", "cube" },
    { "specularExponent", 4U },
    { "specularIntensity", 0.1f },
    { "shading", ShadingType::PIXEL },
    { "dynamic", true },
    { "pos", Vec3f{ 1, 0, -1 } },
    { "rotVel", Vec3f{ 0.1, 0.1, -0.1 } },
    { "texture", "brick_texture" },
    { "normalMap", "brick_normal" }
});
```
