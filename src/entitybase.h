#pragma once

#include "camera.h"

namespace ramiel {

	class EntityBase {
	public:
		bool enabled;
		virtual void draw(Camera& camera) const = 0;
	};

}
