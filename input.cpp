#include <math.h>

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44


void getInput() {
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

	// move forward
	if (((unsigned short)GetKeyState(VK_W)) >> 15) {
		cameraPos.x += dTime * 100.0f * sin(cameraRot.y);
		cameraPos.y += dTime * 100.0f * sin(cameraRot.x);
		cameraPos.z -= dTime * 100.0f * cos(cameraRot.y);
	}

	// move backward
	if (((unsigned short)GetKeyState(VK_S)) >> 15) {
		cameraPos.x -= dTime * 100.0f * sin(cameraRot.y);
		cameraPos.y -= dTime * 100.0f * sin(cameraRot.x);
		cameraPos.z += dTime * 100.0f * cos(cameraRot.y);
	}
}
