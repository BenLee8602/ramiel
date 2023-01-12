#pragma once

#include "vec.h"
#include "rotation.h"

namespace ramiel {

	class Camera {
		Vec2u res;
        Vec2f halfRes;
        size_t bufferSize;
		float fov;
		float focalLength;
        std::vector<Vec3f> color;
        std::vector<float> depth;
	public:
		Vec3f pos;
		Rotation rot;
		Vec3f backgroundColor;
		float znear;
		float zfar;

		Camera() :
			res(vec2u_0),
			halfRes(vec2f_0),
			bufferSize(0),
			fov(1.57f),
			focalLength(0.0f),
			pos(vec3f_0),
			rot(vec3f_0),
			backgroundColor(vec3f_0),
			znear(0.2f),
			zfar(1000.0f)
		{}

		size_t getBufferSize() const;
		const Vec2u& getRes() const;
		void setRes(Vec2u size);

		float getFov() const;
		void setFov(float fov);

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


	class CameraModifier {
	public:
		virtual void run(Camera& camera) const = 0;
		virtual ~CameraModifier() {}
	};

}
