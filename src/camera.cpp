#include <cmath>
#include "graphics.h"
#include "physics.h"

namespace ramiel {

	static float camPosSpeed = 2.0f;
	static float camRotSpeed = 1.57079f;

	Camera::Camera(unsigned fov, float znear, float zfar) {
		this->pos = vec3f_0;
		this->rot = vec3f_0;
		setFov(fov);
		this->znear = znear;
		this->zfar = zfar;
	}

	void Camera::reset() {
		pos = vec3f_0;
		rot = vec3f_0;
	}

	void Camera::setFov(unsigned fov) {
		if (fov) focalLen = (float)graphics::size[X] / (float)fov * 90.0f;
		else focalLen = graphics::size[X];
	}

	const Vec3f& Camera::getpos() const {
		return pos;
	}

	const Vec3f& Camera::getrot() const {
		return rot;
	}

	void Camera::setpos(const Vec3f& pos) {
		this->pos = pos;
	}

	void Camera::setrot(const Vec3f& rot) {
		this->rot = rot;
	}

	void Camera::move(float x, float y, float z) {
		pos[X] += x;
		pos[Y] += y;
		pos[Z] += z;
	}

	void Camera::rotate(float x, float y, float z) {
		rot = { x, y ,z };
	}

	Vec3f Camera::getCameraCoord(Vec3f in) const {
		return rot.rotate(in - pos);
	}

	Vec2 Camera::getScreenCoord(const Vec3f& in) const {
		Vec2 out = { 0 };
		if (in[Z] != 0.0f) {
			out[X] = (int)(in[X] * focalLen / in[Z] + graphics::mid[X]);
			out[Y] = (int)(in[Y] * focalLen / in[Z] + graphics::mid[Y]);
		}
		return out;
	}


	void Camera::setControls(bool controls[12]) {
		const Vec3f& sin = rot.getSin();
		const Vec3f& cos = rot.getCos();
		Vec3f newRot = rot;

		float camPosSpeed_frame = camPosSpeed;
		if (controls[0]) {
			camPosSpeed_frame *= 10.0f;
		}

		// reset pos and rot
		if (controls[1]) {
			reset();
		}

		// move left
		if (controls[2]) {
			pos[X] -= physics::dtime * camPosSpeed_frame * cos[Y];
			pos[Z] -= physics::dtime * camPosSpeed_frame * sin[Y];
		}

		// move right
		if (controls[3]) {
			pos[X] += physics::dtime * camPosSpeed_frame * cos[Y];
			pos[Z] += physics::dtime * camPosSpeed_frame * sin[Y];
		}

		// move down
		if (controls[4]) {
			pos[Y] -= physics::dtime * camPosSpeed_frame;
		}

		// move up
		if (controls[5]) {
			pos[Y] += physics::dtime * camPosSpeed_frame;
		}

		// move backward
		if (controls[6]) {
			pos[X] += physics::dtime * camPosSpeed_frame * sin[Y];
			pos[Y] += physics::dtime * camPosSpeed_frame * -sin[X];
			pos[Z] -= physics::dtime * camPosSpeed_frame * cos[Y];
		}

		// move forward
		if (controls[7]) {
			pos[X] -= physics::dtime * camPosSpeed_frame * sin[Y];
			pos[Y] -= physics::dtime * camPosSpeed_frame * -sin[X];
			pos[Z] += physics::dtime * camPosSpeed_frame * cos[Y];
		}

		// turn right
		if (controls[8]) {
			newRot[Y] -= physics::dtime * camRotSpeed;
		}

		// turn left
		if (controls[9]) {
			newRot[Y] += physics::dtime * camRotSpeed;
		}

		// turn down
		if (controls[10]) {
			if (newRot[X] > -1.57079f) newRot[X] -= physics::dtime * camRotSpeed;
		}

		// turn up
		if (controls[11]) {
			if (newRot[X] < 1.57079f) newRot[X] += physics::dtime * camRotSpeed;
		}

		rot = newRot;
	}

}
