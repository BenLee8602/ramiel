#ifndef INPUT_H_
#define INPUT_H_


#include <math.h>
#include <windows.h>
#include "global.h"

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

#define VK_Q 0x51


bool updateControls() {
	// reset pos and rot
	if (((unsigned short)GetKeyState(VK_Q)) >> 15) {
		cameraPos = { 0.0f };
		cameraRot = { 0.0f };
	}

	// move left
	if (((unsigned short)GetKeyState(VK_A)) >> 15) {
		cameraPos.x -= dTime * 100.0f * cos(cameraRot.y);
		cameraPos.z -= dTime * 100.0f * sin(cameraRot.y);
	}

	// move right
	if (((unsigned short)GetKeyState(VK_D)) >> 15) {
		cameraPos.x += dTime * 100.0f * cos(cameraRot.y);
		cameraPos.z += dTime * 100.0f * sin(cameraRot.y);
	}

	// move down
	if (((unsigned short)GetKeyState(VK_CONTROL)) >> 15) {
		cameraPos.y -= dTime * 100.0f;
	}

	// move up
	if (((unsigned short)GetKeyState(VK_SPACE)) >> 15) {
		cameraPos.y += dTime * 100.0f;
	}

	// move backward
	if (((unsigned short)GetKeyState(VK_S)) >> 15) {
		cameraPos.x += dTime * 100.0f * sin(cameraRot.y);
		cameraPos.y += dTime * 100.0f * sin(cameraRot.x);
		cameraPos.z -= dTime * 100.0f * cos(cameraRot.y);
	}

	// move forward
	if (((unsigned short)GetKeyState(VK_W)) >> 15) {
		cameraPos.x -= dTime * 100.0f * sin(cameraRot.y);
		cameraPos.y -= dTime * 100.0f * sin(cameraRot.x);
		cameraPos.z += dTime * 100.0f * cos(cameraRot.y);
	}

	// turn right
	if (((unsigned short)GetKeyState(VK_RIGHT)) >> 15) {
		cameraRot.y -= dTime * 1.57079f;
	}

	// turn left
	if (((unsigned short)GetKeyState(VK_LEFT)) >> 15) {
		cameraRot.y += dTime * 1.57079f;
	}

	// turn down
	if (((unsigned short)GetKeyState(VK_DOWN)) >> 15) {
		cameraRot.x -= dTime * 1.57079f;
	}

	// turn up
	if (((unsigned short)GetKeyState(VK_UP)) >> 15) {
		cameraRot.x += dTime * 1.57079f;
	}

	return !(((unsigned short)GetKeyState(VK_ESCAPE)) >> 15);
}


#endif
