#include <cmath>
#include "camera.h"

namespace ramiel {

	size_t Camera::getBufferSize() const {
		return bufferSize;
	}

	Vec2u Camera::getResolution() const {
		return res;
	}

	void Camera::setResolution(Vec2u newSize) {
		res = newSize;
		mid = res / 2;
		bufferSize = res[X] * res[Y];
		color = std::vector<Vec3f>(bufferSize);
		depth = std::vector<float>(bufferSize);
	}


	void Camera::resetBuffers() {
		std::fill(color.begin(), color.end(), backgroundColor);
		std::fill(depth.begin(), depth.end(), zfar);
		
	}


	Vec3f Camera::getCameraCoord(const Vec3f& in) const {
		return rot.rotate(in - pos);
	}


	Vec2f Camera::getScreenCoord(const Vec3f& in) const {
		Vec2f out = { 0 };
		if (in[Z] != 0.0f) {
			out[X] = in[X] * focalLength / in[Z] + mid[X];
			out[Y] = in[Y] * focalLength / in[Z] + mid[Y];
		}
		return out;
	}


	Camera::ColorBufferIterator Camera::getColorBuffer() {
		return color.begin();
	}

	Camera::DepthBufferIterator Camera::getDepthBuffer() {
		return depth.begin();
	}


	void Camera::clampColorBuffer() {
		for (size_t i = 0; i < bufferSize; ++i) c_min(color[i]);
	}


	void Camera::getFrameDEC(int* frame) const {
		for (size_t i = 0; i < bufferSize; ++i) {
			frame[i] = (
				((int)color[i][R] << 16) +
				((int)color[i][G] <<  8) +
				((int)color[i][B])
			);
		}
	}

	void Camera::getFrameRGB(uint8_t* frame) const {
		uint8_t* f = frame;
		for (size_t i = 0; i < bufferSize; ++i) {
			*f++ = color[i][R];
			*f++ = color[i][G];
			*f++ = color[i][B];
		}
	}

}
