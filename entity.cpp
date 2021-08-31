#include <string>
#include <fstream>
#include <sstream>
#include "entity.h"
#include "render.h"

namespace bl {

	Entity::Entity(const char* filename, const Vec3f& pos) {
		std::vector<Vec3f> points;

		std::string ltr = "";
		std::ifstream file(filename);

		while (file >> ltr) {
			// vertex data
			if (ltr == "v") {
				Vec3f tempPt;
				file >> tempPt.x >> tempPt.y >> tempPt.z;
				points.push_back(tempPt + pos);
			}

			// triangle data
			else if (ltr == "f") {
				Vec3f tempTri[3];
				for (int a = 0; a < 3; a++) {
					std::string nums = "";
					std::stringstream stream;
					int index;

					file >> nums;
					stream << nums;
					stream >> index;
					tempTri[a] = points[index - 1];
				}
				triangles.push_back(Triangle(tempTri[0], tempTri[1], tempTri[2]));
			}
		}
		file.close();
	}


	void Entity::draw() const {
		for (auto& t : triangles) {
			t.draw();
		}
	}

}
