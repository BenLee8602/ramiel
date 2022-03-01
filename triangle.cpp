#include <algorithm>
#include "graphics.h"

namespace bl {

	Triangle::Triangle(size_t pts[3]) {
		this->pts[0] = pts[0];
		this->pts[1] = pts[1];
		this->pts[2] = pts[2];
	}


	size_t Triangle::operator[](size_t index) const {
		return pts[index];
	}

}
