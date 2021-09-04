#ifndef BL_LIGHT_H
#define BL_LIGHT_H

#include "vec.h"

namespace bl {

	class Light {
	protected:
		Vec3f source;
		Vec3f color;
	public:
		Light(Vec3f& source, Vec3f& color);
		virtual void getLight(Vec3f& rgb, Vec3f& normal) const;
	};

	int rgbToDec(Vec3f& rgb);

}

#endif
