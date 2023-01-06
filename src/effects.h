#pragma once

#include "camera.h"

namespace ramiel {

	typedef CameraModifier Effect;


	class Hdr : public Effect {
	public:
		virtual void run(Camera& camera) const override;
	};


	class Fog : public Effect {
		Vec3f fogColor;
		float fogStart;
		float fogEnd;
		float fogFactor;
	public:
		Fog(
			Vec3f fogColor,
			float fogStart,
			float fogEnd
		) :
			fogColor(fogColor),
			fogStart(fogStart),
			fogEnd(fogEnd),
			fogFactor(1.0f / (fogEnd - fogStart))
		{}
		virtual void run(Camera& camera) const override;
	};

}
