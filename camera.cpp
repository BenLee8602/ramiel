#include <windows.h>
#include <cmath>
#include "graphics.h"

#define KEY_Q     0x51
#define KEY_W     0x57
#define KEY_A     0x41
#define KEY_S     0x53
#define KEY_D     0x44
#define KEY_UP    0x26
#define KEY_DOWN  0x28
#define KEY_LEFT  0x25
#define KEY_RIGHT 0x27
#define KEY_SHIFT 0x10
#define KEY_CTRL  0x11
#define KEY_SPACE 0x20


namespace bl {

	static float camPosSpeed = 2.0f;
	static float camRotSpeed = 1.57079f;

	Camera::Camera() {
		pos = { 0.0f };
		rot = { 0.0f };
		calcTrigValues();
		focalLen = GraphicsBL::size[X];
	}

	Camera::Camera(const Vec3f& pos, const Vec3f& rot, int fov) {
		this->pos = pos;
		this->rot = rot;
		calcTrigValues();
		setFov(fov);
	}

	void Camera::calcTrigValues() {
		sin[X] = sinf(rot[X]);
		sin[Y] = sinf(rot[Y]);
		sin[Z] = sinf(rot[Z]);

		cos[X] = cosf(rot[X]);
		cos[Y] = cosf(rot[Y]);
		cos[Z] = cosf(rot[Z]);
	}

	void Camera::reset() {
		pos = { 0.0f };
		rot = { 0.0f };
	}

	void Camera::setFov(int fov) {
		if (fov) focalLen = (GraphicsBL::size[X] / fov) * 90;
		else focalLen = GraphicsBL::size[X];
	}

	const Vec3f& Camera::getpos() const {
		return pos;
	}

	const Vec3f& Camera::getrot() const {
		return rot;
	}

	void Camera::move(float x, float y, float z) {
		pos[X] += x;
		pos[Y] += y;
		pos[Z] += z;
	}

	void Camera::rotate(float x, float y, float z) {
		rot[X] += x;
		rot[Y] += y;
		rot[Z] += z;
	}

	Vec3f Camera::getCameraCoord(Vec3f in) const {
		// translate
		Vec3f out = in -= pos;

		// z rot
		out[X] = in[X] * cos[Z] + in[Y] * -sin[Z];
		out[Y] = in[X] * sin[Z] + in[Y] * cos[Z];
		in = out;

		// y rot
		out[X] = in[X] * cos[Y] + in[Z] * sin[Y];
		out[Z] = in[X] * -sin[Y] + in[Z] * cos[Y];
		in = out;

		// x rot
		out[Y] = in[Y] * cos[X] + in[Z] * -sin[X];
		out[Z] = in[Y] * sin[X] + in[Z] * cos[X];

		return out;
	}

	Vec2 Camera::getScreenCoord(const Vec3f& in) const {
		Vec2 out = { 0 };
		if (in[Z] != 0.0f) {
			out[X] = (int)(in[X] * focalLen / in[Z] + GraphicsBL::mid[X]);
			out[Y] = (int)(in[Y] * focalLen / in[Z] + GraphicsBL::mid[Y]);
		}
		return out;
	}

	void Camera::getControls() {
		float camPosSpeed_frame = camPosSpeed;
		if (((unsigned short)GetKeyState(KEY_SHIFT)) >> 15) {
			camPosSpeed_frame *= 10.0f;
		}

		// reset pos and rot
		if (((unsigned short)GetKeyState(KEY_Q)) >> 15) {
			reset();
		}
		
		// move left
		if (((unsigned short)GetKeyState(KEY_A)) >> 15) {
			pos[X] -= GraphicsBL::dtime * camPosSpeed_frame * cos[Y];
			pos[Z] -= GraphicsBL::dtime * camPosSpeed_frame * sin[Y];
		}

		// move right
		if (((unsigned short)GetKeyState(KEY_D)) >> 15) {
			pos[X] += GraphicsBL::dtime * camPosSpeed_frame * cos[Y];
			pos[Z] += GraphicsBL::dtime * camPosSpeed_frame * sin[Y];
		}

		// move down
		if (((unsigned short)GetKeyState(KEY_CTRL)) >> 15) {
			pos[Y] -= GraphicsBL::dtime * camPosSpeed_frame;
		}

		// move up
		if (((unsigned short)GetKeyState(KEY_SPACE)) >> 15) {
			pos[Y] += GraphicsBL::dtime * camPosSpeed_frame;
		}

		// move backward
		if (((unsigned short)GetKeyState(KEY_S)) >> 15) {
			pos[X] += GraphicsBL::dtime * camPosSpeed_frame * sin[Y];
			pos[Y] += GraphicsBL::dtime * camPosSpeed_frame * -sin[X];
			pos[Z] -= GraphicsBL::dtime * camPosSpeed_frame * cos[Y];
		}

		// move forward
		if (((unsigned short)GetKeyState(KEY_W)) >> 15) {
			pos[X] -= GraphicsBL::dtime * camPosSpeed_frame * sin[Y];
			pos[Y] -= GraphicsBL::dtime * camPosSpeed_frame * -sin[X];
			pos[Z] += GraphicsBL::dtime * camPosSpeed_frame * cos[Y];
		}

		// turn right
		if (((unsigned short)GetKeyState(KEY_RIGHT)) >> 15) {
			rot[Y] -= GraphicsBL::dtime * camRotSpeed;
		}

		// turn left
		if (((unsigned short)GetKeyState(KEY_LEFT)) >> 15) {
			rot[Y] += GraphicsBL::dtime * camRotSpeed;
		}

		// turn down
		if (((unsigned short)GetKeyState(KEY_DOWN)) >> 15) {
			if (rot[X] > -1.57079f) rot[X] -= GraphicsBL::dtime * camRotSpeed;
		}

		// turn up
		if (((unsigned short)GetKeyState(KEY_UP)) >> 15) {
			if (rot[X] < 1.57079f) rot[X] += GraphicsBL::dtime * camRotSpeed;
		}
	}

}
