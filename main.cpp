#include <windows.h>

void* memory;

int winSizeX;
int winSizeY;

bool run = true;
int bufferSize;
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
			bufferSize = winSizeX * winSizeY * sizeof(unsigned int);

			if (memory) VirtualFree(memory, 0, MEM_RELEASE);
			memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

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


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
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

	// main loop
	while (run) {
		// window message handling
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// update frame
		StretchDIBits(
			hdc, 0, 0,
			winSizeX, winSizeY,
			0, 0,
			winSizeX, winSizeY,
			memory, &bitmapInfo,
			DIB_RGB_COLORS, SRCCOPY
		);
	}

	return 0;
}
