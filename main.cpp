#include <windows.h>
#include <chrono>
#include "render.h"
using namespace bl;

void* pixels;
int winSizeX = 1280;
int winSizeY = 720;
bool run = true;
BITMAPINFO bitmapInfo;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;

	switch (uMsg) {
	// window size changed
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);

		winSizeX = rect.right - rect.left;
		winSizeY = rect.bottom - rect.top;
		int bufferSize = winSizeX * winSizeY * sizeof(unsigned int);

		if (pixels) VirtualFree(pixels, 0, MEM_RELEASE);
		pixels = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		RenderBL::setBuffer(pixels, winSizeX, winSizeY);

		bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
		bitmapInfo.bmiHeader.biWidth = winSizeX;
		bitmapInfo.bmiHeader.biHeight = winSizeY;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression = BI_RGB;
	} break;

	// exit
	case WM_CLOSE: run = false; break;
	case WM_DESTROY: run = false; break;

	default: result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}


int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// register window class
	WNDCLASS winClass = {};
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpszClassName = "Window";
	winClass.lpfnWndProc = WindowProc;
	RegisterClass(&winClass);

	// create window
	HWND window = CreateWindowA(
		winClass.lpszClassName,
		"Window",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		winSizeX, winSizeY, 0, 0,
		hInstance,
		0
	);
	HDC hdc = GetDC(window);

	// time
	std::chrono::steady_clock::time_point frameStart;
	std::chrono::steady_clock::time_point frameEnd;
	float dtime = 0.0f;

	// add objects and lighting to scene
	RenderBL::addLight(new Light_Sp());
	RenderBL::addEntity(new Entity("model/cube2.obj", ShadingType::PIXEL, { 0, 0, 1 }));
	
	// main loop
	while (run) {
		frameStart = std::chrono::steady_clock::now();
		
		// window message handling
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// render frame
		RenderBL::renderFrame(dtime);

		// update frame
		StretchDIBits(
			hdc, 0, 0,
			winSizeX, winSizeY,
			0, 0,
			winSizeX, winSizeY,
			pixels, &bitmapInfo,
			DIB_RGB_COLORS, SRCCOPY
		);

		frameEnd = std::chrono::steady_clock::now();
		dtime = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart).count() / 1000000.0f;
	}

	return 0;
}
