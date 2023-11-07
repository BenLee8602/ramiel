# ramiel

ramiel is a 3D graphics + physics library. All graphics + physics related functionality was 
written without the use of any external libraries or frameworks.


## examples

![Mountains](https://github.com/BenLee8602/ramiel/blob/master/screenshots/perlin_mountain.PNG?raw=true)
A mountain scene made using the following entities, lighting, and effects:
```cpp
Scene scene;
scene.camera.setRes({ width, height });

scene.loadMesh<Vertex_Mesh>("examples/assets/models/terrain.obj", "terrain");

scene.ambientLight = { 100, 80, 100 };
scene.addLight(new DirectionalLight({ 155, 40, 0 }, 1.0f, { -10, 1, 0 }));
scene.addEntity<Vertex_Mesh>(
    "terrain",
    VS_PerTri(scene.camera, new Transform({ -64, 0, -64 })),
    PS_PerTri(scene.getLightingList(8, 1.0f), vec3f_255)
);
scene.addEffect(new Fog({ 150, 110, 110 }, 20, 100));
```

![Lighting](https://github.com/BenLee8602/ramiel/blob/master/screenshots/ramiel.PNG?raw=true)
A test scene showing per-pixel lighting, a spotlight, and specular reflection:
```cpp
Scene scene;
scene.camera.setRes({ width, height });

scene.loadMesh<Vertex_Mesh>("examples/assets/models/ramiel.obj", "ramiel");

scene.ambientLight = { 50, 50, 50 };
scene.addLight(new SpotLight(vec3f_255, 4.0f, { -1, 2, 3 }, { 1, -2, 1 }, 0.1f, 1.0f, 50.0f));
scene.addEntity<Vertex_Mesh>(
    "ramiel",
    VS_PerPixel(scene.camera, new Transform({ 0, 0, 4 })),
    PS_PerPixel(scene.getLightingList(256, 1.0f), { 70, 135, 255 })
);
```

![Texture Mapping](https://github.com/BenLee8602/ramiel/blob/master/screenshots/brick.PNG?raw=true)
A test scene showing 4 cubes, with textures and normal mapping.
* Top: no texture or normal map
* Left: textured only
* Right: normal mapped only
* Bottom: textured and normal mapped
```cpp
Scene scene;
scene.camera.setRes({ width, height });

scene.loadMesh<Vertex_Mesh_TN>("examples/assets/models/cube.obj", "cube", true, true);
scene.loadTexture("examples/assets/textures/brickwall_texture.jpg", "brick_texture");
scene.loadNormalMap("examples/assets/textures/brickwall_normal.jpg", "brick_normal");

scene.ambientLight = { 25, 25, 25 };
scene.addLight(new PointLight(vec3f_255, 2.0f, { 0, 1, 0 }, 0.2));

// no texture or normal map
Dynamics* p1 = new Dynamics({ -1, 0, 1 }, vec3f_0, vec3f_0, { -0.1,  0.1,  0.1 });
scene.addEntity<Vertex_Mesh_TN>(
    "cube",
    VS_PerPixel(scene.camera, p1),
    PS_PerPixel(
        scene.getLightingList(4, 0.1),
        { 100, 90, 70 }
    )
);

// texture only
Dynamics* p2 = new Dynamics({ 1, 0, 1 }, vec3f_0, vec3f_0, { 0.1, 0.1, 0.1 });
scene.addEntity<Vertex_Mesh_TN>(
    "cube",
    VS_PerPixel_Textured(scene.camera, p2),
    PS_PerPixel_Textured(
        scene.getLightingList(4, 0.1),
        scene.getTexture("brick_texture")
    )
);

// normal map only
Dynamics* p3 = new Dynamics({ -1, 0, -1 }, vec3f_0, vec3f_0, { -0.1, 0.1, -0.1 });
scene.addEntity<Vertex_Mesh_TN>(
    "cube",
    VS_PerPixel_Textured(scene.camera, p3),
    PS_PerPixel_NormalMapped(
        scene.getLightingList(4, 0.1),
        scene.getTexture("brick_normal"),
        { 100, 90, 70 }
    )
);

// both texture and normal map
Dynamics* p4 = new Dynamics({ 1, 0, -1 }, vec3f_0, vec3f_0, { 0.1, 0.1, -0.1 });
scene.addEntity<Vertex_Mesh_TN>(
    "cube",
    VS_PerPixel_Textured(scene.camera, p4),
    PS_PerPixel_Textured_NormalMapped(
        scene.getLightingList(4, 0.1),
        scene.getTexture("brick_texture"),
        scene.getTexture("brick_normal")
    )
);

scene.addDynamicObject(p1);
scene.addDynamicObject(p2);
scene.addDynamicObject(p3);
scene.addDynamicObject(p4);
```


## build

clone the repo
```
git clone https://github.com/BenLee8602/ramiel.git ramiel
cd ramiel
```

generate project files using cmake. will use mingw makefiles for this example
```
cmake -B build -S . -G "MinGW Makefiles"
mingw32-make -C build
```

you should now see `ramiel/build/src/libramiel.a`  
heres how you can build the example in `ramiel/examples/glfw_example`. make sure you have glfw installed
```
g++ -Iinclude -Ipath/to/glfw examples/glfw_example/main.cpp build/src/libramiel.a path/to/glfw/libglfw3.a -lgdi32 -lopengl32 -o ramiel.exe
```
