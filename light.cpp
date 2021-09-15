#include <algorithm>
#include "light.h"
#include "render.h"

namespace bl {

	Light_Dir::Light_Dir(Vec3f& source, Vec3f& color) {
		this->direction = source;
		this->color = color;
	}

	void Light_Dir::getLight(Vec3f& rgb, Vec3f& normal) const {
		rgb += color * dirSimilarity(normal, direction);
	}


	Light_Pt::Light_Pt(Vec3f& source, Vec3f& color, float falloff) {
		this->source = source;
		this->color = color;
		this->falloff = falloff;
	}

	void Light_Pt::getLight(Vertex& vertex) const {
		Vec3f vec = vertex.pos - source;
		float d = vec.getMagnitude();
		float f = 1.0f / (falloff * d * d + 1.0f);
		vertex.color += color * f * (1.0f - dirSimilarity(vec, vertex.normal));
	}

	void Light_Pt::moveSource(const Vec3f& pos) {
		source = pos;
	}


	int rgbToDec(Vec3f& rgb) {
		return ((int)rgb.x << 16) + ((int)rgb.y << 8) + (int)rgb.z;
	}

}