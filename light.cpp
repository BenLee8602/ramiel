#include <algorithm>
#include "light.h"

namespace bl {

	Light::Light(Vec3f& source, Vec3f& color) {
		this->source = source;
		this->color = color;
	}

	void Light::getLight(Vec3f& rgb, Vec3f& normal) const {
		rgb += color * dirSimilarity(normal, source);
		rgb.x = std::min(rgb.x, 255.0f);
		rgb.y = std::min(rgb.y, 255.0f);
		rgb.z = std::min(rgb.z, 255.0f);
	}

	int rgbToDec(Vec3f& rgb) {
		return ((int)rgb.x << 16) + ((int)rgb.y << 8) + (int)rgb.z;
	}

}
