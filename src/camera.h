#pragma once

#include "vec.h"
#include "rotation.h"

namespace ramiel {

	class Camera {
		Vec2u res;
        Vec2u mid;
        size_t bufferSize;
        std::vector<Vec3f> color;
        std::vector<float> depth;
	public:
		Vec3f pos;
		Rotation rot;
		float focalLength;
		Vec3f backgroundColor;
		float znear;
		float zfar;

		Camera() :
			res(vec2u_0),
			mid(vec2u_0),
			bufferSize(0),
			pos(vec3f_0),
			rot(vec3f_0),
			focalLength(0.0f),
			backgroundColor(vec3f_0),
			znear(0.2f),
			zfar(1000.0f)
		{}

		size_t getBufferSize() const;
		Vec2u getResolution() const;
		void setResolution(Vec2u size);

		void resetBuffers();

		Vec3f getCameraCoord(const Vec3f& in) const;
		Vec2f getScreenCoord(const Vec3f& in) const;

		typedef std::vector<Vec3f>::iterator ColorBufferIterator;
		typedef std::vector<float>::iterator DepthBufferIterator;
		ColorBufferIterator getColorBuffer();
		DepthBufferIterator getDepthBuffer();

		void clampColorBuffer();

		void getFrameDEC(int* frame) const;
		void getFrameRGB(uint8_t* frame) const;
	};

}
