#include <windows.h>
#include <cmath>
#include "camera.h"
#include "render.h"

#define CAM_POS_SPEED 2
#define CAM_ROT_SPEED 1.57079f

#define KEY_Q     0x51
#define KEY_W     0x57
#define KEY_A     0x41
#define KEY_S     0x53
#define KEY_D     0x44
#define KEY_UP    0x26
#define KEY_DOWN  0x28
#define KEY_LEFT  0x25
#define KEY_RIGHT 0x27
#define KEY_CTRL  0x11
#define KEY_SPACE 0x20

namespace bl {

	Camera::Camera() {
		pos = { 0.0f };
		rot = { 0.0f };
		calcTrigValues();
		focalLen = RenderBL::size.x;
	}

	Camera::Camera(const Vec3f& pos, const Vec3f& rot, int fov) {
		this->pos = pos;
		this->rot = rot;
		calcTrigValues();
		setFov(fov);
	}

	void Camera::calcTrigValues() {
		sin.x = sinf(rot.x);
		sin.y = sinf(rot.y);
		sin.z = sinf(rot.z);

		cos.x = cosf(rot.x);
		cos.y = cosf(rot.y);
		cos.z = cosf(rot.z);
	}

	void Camera::reset() {
		pos = { 0.0f };
		rot = { 0.0f };
	}

	void Camera::setFov(int fov) {
		if (fov) focalLen = (RenderBL::size.x / fov) * 90;
		else focalLen = RenderBL::size.x;
	}

	void Camera::setpos(float x, float y, float z) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
	}

	void Camera::setrot(float x, float y, float z) {
		rot.x = x;
		rot.y = y;
		rot.z = z;
	}

	void Camera::move(float x, float y, float z) {
		pos.x += x;
		pos.y += y;
		pos.z += z;
	}

	void Camera::rotate(float x, float y, float z) {
		rot.x += x;
		rot.y += y;
		rot.z += z;
	}

	Vec3f Camera::getCameraCoord(Vec3f in) const {
		// translate
		Vec3f out = in -= pos;

		// z rot
		out.x = in.x * cos.z + in.y * -sin.z;
		out.y = in.x * sin.z + in.y * cos.z;
		in = out;

		// y rot
		out.x = in.x * cos.y + in.z * sin.y;
		out.z = in.x * -sin.y + in.z * cos.y;
		in = out;

		// x rot
		out.y = in.y * cos.x + in.z * -sin.x;
		out.z = in.y * sin.x + in.z * cos.x;

		return out;
	}

	Vec2 Camera::getScreenCoord(const Vec3f& in) const {
		Vec2 out = { 0 };
		if (in.z != 0.0f) {
			out.x = (int)(in.x * focalLen / in.z + RenderBL::mid.x);
			out.y = (int)(in.y * focalLen / in.z + RenderBL::mid.y);
		}
		return out;
	}

	void Camera::getControls() {
		// reset pos and rot
		if (((unsigned short)GetKeyState(KEY_Q)) >> 15) {
			reset();
		}
		
		// move left
		if (((unsigned short)GetKeyState(KEY_A)) >> 15) {
			pos.x -= RenderBL::dtime * CAM_POS_SPEED * cos.y;
			pos.z -= RenderBL::dtime * CAM_POS_SPEED * sin.y;
		}

		// move right
		if (((unsigned short)GetKeyState(KEY_D)) >> 15) {
			pos.x += RenderBL::dtime * CAM_POS_SPEED * cos.y;
			pos.z += RenderBL::dtime * CAM_POS_SPEED * sin.y;
		}

		// move down
		if (((unsigned short)GetKeyState(KEY_CTRL)) >> 15) {
			pos.y -= RenderBL::dtime * CAM_POS_SPEED;
		}

		// move up
		if (((unsigned short)GetKeyState(KEY_SPACE)) >> 15) {
			pos.y += RenderBL::dtime * CAM_POS_SPEED;
		}

		// move backward
		if (((unsigned short)GetKeyState(KEY_S)) >> 15) {
			pos.x += RenderBL::dtime * CAM_POS_SPEED * sin.y;
			pos.y += RenderBL::dtime * CAM_POS_SPEED * -sin.x;
			pos.z -= RenderBL::dtime * CAM_POS_SPEED * cos.y;
		}

		// move forward
		if (((unsigned short)GetKeyState(KEY_W)) >> 15) {
			pos.x -= RenderBL::dtime * CAM_POS_SPEED * sin.y;
			pos.y -= RenderBL::dtime * CAM_POS_SPEED * -sin.x;
			pos.z += RenderBL::dtime * CAM_POS_SPEED * cos.y;
		}

		// turn right
		if (((unsigned short)GetKeyState(KEY_RIGHT)) >> 15) {
			rot.y -= RenderBL::dtime * CAM_ROT_SPEED;
		}

		// turn left
		if (((unsigned short)GetKeyState(KEY_LEFT)) >> 15) {
			rot.y += RenderBL::dtime * CAM_ROT_SPEED;
		}

		// turn down
		if (((unsigned short)GetKeyState(KEY_DOWN)) >> 15) {
			if (rot.x > -1.57079f) rot.x -= RenderBL::dtime * CAM_ROT_SPEED;
		}

		// turn up
		if (((unsigned short)GetKeyState(KEY_UP)) >> 15) {
			if (rot.x < 1.57079f) rot.x += RenderBL::dtime * CAM_ROT_SPEED;
		}
	}

}
