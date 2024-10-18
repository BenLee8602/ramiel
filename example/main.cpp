#include <chrono>
#include <windows.h>

#include <ramiel/ramiel.h>
using namespace ramiel;

bool run = true;

constexpr int width = 1280;
constexpr int height = 720;

constexpr float cameraSpeedNormal = 2.0f;
constexpr float cameraSpeedFaster = 10.0f;
constexpr float cameraRotationSpeed = 1.57079f;


bool getKey(int key) {
    return GetKeyState(key) & 0x8000;
}

void cameraControls(float dtime) {
    // reset pos and rot
    if (getKey('Q')) {
        setPos(Vec3f());
        setRot(Vec3f());
        return;
    }

    // move faster
    float cameraSpeed = (
        getKey(VK_LSHIFT) ?
        cameraSpeedFaster : cameraSpeedNormal
    );
    float dPos = cameraSpeed * dtime;
    float dRot = cameraRotationSpeed * dtime;

    Vec3f pos = getPos();
    Vec3f rot = getRot();

    // move left
    if (getKey('A')) {
        pos[X] -= dPos * std::cos(rot[Y]);
        pos[Z] -= dPos * std::sin(rot[Y]);
    }

    // move right
    if (getKey('D')) {
        pos[X] += dPos * std::cos(rot[Y]);
        pos[Z] += dPos * std::sin(rot[Y]);
    }

    // move down
    if (getKey(VK_LCONTROL)) {
        pos[Y] -= dPos;
    }

    // move up
    if (getKey(VK_SPACE)) {
        pos[Y] += dPos;
    }

    // move backward
    if (getKey('S')) {
        pos[X] += dPos * std::sin( rot[Y]);
        pos[Y] += dPos * std::sin(-rot[X]);
        pos[Z] -= dPos * std::cos( rot[Y]);
    }

    // move forward
    if (getKey('W')) {
        pos[X] -= dPos * std::sin( rot[Y]);
        pos[Y] -= dPos * std::sin(-rot[X]);
        pos[Z] += dPos * std::cos( rot[Y]);
    }

    // turn down
    if (getKey(VK_DOWN)) {
        rot[X] = std::max(rot[X] - dRot, -1.57079f);
    }

    // turn up
    if (getKey(VK_UP)) {
        rot[X] = std::min(rot[X] + dRot, 1.57079f);
    }

    // turn right
    if (getKey(VK_RIGHT)) {
        rot[Y] -= dRot;
    }

    // turn left
    if (getKey(VK_LEFT)) {
        rot[Y] += dRot;
    }

    setPos(pos);
    setRot(rot);
}


void getFrameRGB(uint8_t* frame) {
    auto color = getColorBuffer();
    for (size_t i = 0; i < getBufferSize(); ++i) {
        *frame++ = color[i][B];
        *frame++ = color[i][G];
        *frame++ = color[i][R];
    }
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        run = false;
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int main() {
    HINSTANCE instance;

    WNDCLASS windowClass = {};
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = "RamielWindowClass";
    RegisterClass(&windowClass);

    HWND window = CreateWindowEx(
        0,
        "RamielWindowClass",
        "ramiel",
        WS_OVERLAPPEDWINDOW,
        0, 0, width, height,
        nullptr,
        nullptr,
        instance,
        nullptr
    );
    if (!window) return 0;

    HDC deviceContext = GetDC(window);

    BITMAPINFO bitmapInfo;
    ZeroMemory(&bitmapInfo, sizeof(bitmapInfo));
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = height;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 24;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    ShowWindow(window, SW_SHOW);

    setRes({ width, height });

    loadMesh<MeshVertex>("example/assets/models/terrain.obj", "terrain");
    loadMesh<MeshVertexT>("example/assets/models/cube.obj", "cube");

    loadTexture("example/assets/textures/brickwall_texture.jpg", "brickwall", rgb1);

    setAmbientLight({ 100, 80, 100 });
    addLight(new DirectionalLight({ 155, 40, 0 }, 1.0f, { -10, 1, 0 }));
    addEntity<MeshVertex>(
        "terrain",
        VS_PerTri(new PhysicsObject({ -64, 0, -64 })),
        PS_PerTri(getLightingList(8, 1.0f), Vec3f{ 255, 255, 255 })
    );
    addEntity<MeshVertexT>(
        "cube",
        VS_PerPixel_Textured(new PhysicsObject()),
        PS_PerPixel_Textured(getLightingList(8, 1.0f), getTexture("brickwall"))
    );
    addEffect(new Fog({ 150, 110, 110 }, 20, 100));

    uint8_t* frame = new uint8_t[width * height * 3];
    auto frameTimeStart = std::chrono::steady_clock::now();
    auto frameTimeEnd = std::chrono::steady_clock::now();
    
    while (run) {
        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        frameTimeEnd = std::chrono::steady_clock::now();
        double dtime = (frameTimeEnd - frameTimeStart).count() / 1e9;
        frameTimeStart = frameTimeEnd;

        cameraControls((float)dtime);
        simulatePhysics(dtime);
        renderFrame();
        getFrameRGB(frame);

        StretchDIBits(
            deviceContext,
            0, 0, width, height,
            0, 0, width, height,
            frame, &bitmapInfo,
            DIB_RGB_COLORS, SRCCOPY
        );
    }

    DestroyWindow(window);
    destroy();
    delete[] frame;
    return 0;
}
