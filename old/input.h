#ifndef INPUT_H_
#define INPUT_H_


#include <windows.h>
#include "global.h"

#define CAM_POS_SPEED 2
#define CAM_ROT_SPEED 1.57079f

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

#define VK_Q 0x51


bool updateControls() {
	// reset pos and rot
	if (((unsigned short)GetKeyState(VK_Q)) >> 15) {
		cam.resetCameraState();
	}

	// move left
	if (((unsigned short)GetKeyState(VK_A)) >> 15) {
		cam.pos.x -= dTime * CAM_POS_SPEED * cam.cosy;
		cam.pos.z -= dTime * CAM_POS_SPEED * cam.siny;
	}

	// move right
	if (((unsigned short)GetKeyState(VK_D)) >> 15) {
		cam.pos.x += dTime * CAM_POS_SPEED * cam.cosy;
		cam.pos.z += dTime * CAM_POS_SPEED * cam.siny;
	}

	// move down
	if (((unsigned short)GetKeyState(VK_CONTROL)) >> 15) {
		cam.pos.y -= dTime * CAM_POS_SPEED;
	}

	// move up
	if (((unsigned short)GetKeyState(VK_SPACE)) >> 15) {
		cam.pos.y += dTime * CAM_POS_SPEED;
	}

	// move backward
	if (((unsigned short)GetKeyState(VK_S)) >> 15) {
		cam.pos.x += dTime * CAM_POS_SPEED *  cam.siny;
		cam.pos.y += dTime * CAM_POS_SPEED * -cam.sinx;
		cam.pos.z -= dTime * CAM_POS_SPEED *  cam.cosy;
	}

	// move forward
	if (((unsigned short)GetKeyState(VK_W)) >> 15) {
		cam.pos.x -= dTime * CAM_POS_SPEED *  cam.siny;
		cam.pos.y -= dTime * CAM_POS_SPEED * -cam.sinx;
		cam.pos.z += dTime * CAM_POS_SPEED *  cam.cosy;
	}

	// turn right
	if (((unsigned short)GetKeyState(VK_RIGHT)) >> 15) {
		cam.rot.y -= dTime * CAM_ROT_SPEED;
	}

	// turn left
	if (((unsigned short)GetKeyState(VK_LEFT)) >> 15) {
		cam.rot.y += dTime * CAM_ROT_SPEED;
	}

	// turn down
	if (((unsigned short)GetKeyState(VK_DOWN)) >> 15) {
		if (cam.rot.x > -1.57079f) cam.rot.x -= dTime * CAM_ROT_SPEED;
	}

	// turn up
	if (((unsigned short)GetKeyState(VK_UP)) >> 15) {
		if (cam.rot.x < 1.57079f) cam.rot.x += dTime * CAM_ROT_SPEED;
	}

	return !(((unsigned short)GetKeyState(VK_ESCAPE)) >> 15);
}


#endif
