#ifndef BL_LIGHT_H
#define BL_LIGHT_H

#include "vec.h"

namespace bl {

	class Light_Dir {
		Vec3f direction;
		Vec3f color;
	public:
		Light_Dir(Vec3f& direction, Vec3f& color);
		void getLight(Vec3f& rgb, Vec3f& normal) const;
	};

	class Light_Pt {
		Vec3f source;
		Vec3f color;
		float falloff; // light intensity falloff coefficient
	public:
		Light_Pt(Vec3f& source, Vec3f& color, float falloff);
		void getLight(Vertex& vertex) const;
		void moveSource(const Vec3f& pos);
	};

	int rgbToDec(Vec3f& rgb);

}

#endif