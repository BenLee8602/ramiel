#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "global.h"
#include "draw.h"
#include "render.h"
#include "input.h"

void* memory;
void* zBuffer;

bool run = true;
int bufferSize;

int winSizeX;
int winSizeY;
float winMidX;
float winMidY;

float time;
float dTime;

BITMAPINFO bitmapInfo;


float getTime(LARGE_INTEGER* frameStartTime, LARGE_INTEGER* frameEndTime) {
	LARGE_INTEGER runsPerSec;
	QueryPerformanceFrequency(&runsPerSec);
	return float(frameEndTime->QuadPart - frameStartTime->QuadPart) / runsPerSec.QuadPart;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT result = 0;
	switch (uMsg) {
		// window size changed
		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
	
			winSizeX = rect.right - rect.left;
			winSizeY = rect.bottom - rect.top;
			winMidX = (float)winSizeX / 2.0f;
			winMidY = (float)winSizeY / 2.0f;
			bufferSize = winSizeX * winSizeY * sizeof(unsigned int);
			
			if (memory) VirtualFree(memory, 0, MEM_RELEASE);
			memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			if (zBuffer) VirtualFree(zBuffer, 0, MEM_RELEASE);
			zBuffer = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
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
	
		// no message
		default: result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	char title[23] = "";
	float fps;

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
		1280, 720, 0, 0,
		hInstance,
		0
	);
	HDC hdc = GetDC(window);

	// inititalize time
	time = 0.0f;
	dTime = 0.016666f;
	LARGE_INTEGER frameStartTime;
	LARGE_INTEGER frameEndTime;
	QueryPerformanceCounter(&frameStartTime);

	// initialize world
	renderStart();

	// main loop
	while (run) {
		// window message handling
		run = updateControls();
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// render frame
		memset(memory, 0, bufferSize);
		renderMain();

		// update frame
		StretchDIBits(
			hdc, 0, 0, 
			winSizeX, winSizeY, 
			0, 0, 
			winSizeX, winSizeY, 
			memory, &bitmapInfo, 
			DIB_RGB_COLORS, SRCCOPY
		);

		// update time
		QueryPerformanceCounter(&frameEndTime);
		dTime = getTime(&frameStartTime, &frameEndTime);
		frameStartTime = frameEndTime;
		time += dTime;
		
		// display fps
		if (time > 1) {
			time = 0;
			fps = 1.0f / dTime;
			sprintf_s(title, "Window - %.2f FPS", fps);
			SetWindowTextA(window, title);
		}
	}

	return 0;
}
