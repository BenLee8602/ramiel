#include <string>
#include <fstream>
#include <sstream>
#include "entity.h"
#include "render.h"

namespace bl {

	Entity::Entity(const char* filename, const Vec3f& pos) {
		std::vector<Vec3f> pts;
		std::ifstream file(filename);
		std::string line;

		while (getline(file, line)) {
			// insert line content into stream
			std::stringstream stream;
			stream << line;

			// determine type of data on current line
			std::string ltr;
			stream >> ltr;

			// vertex data
			if (ltr == "v") {
				Vec3f tempPt;
				stream >> tempPt.x >> tempPt.y >> tempPt.z;
				pts.push_back(tempPt + pos);
			}

			// triangle data
			else if (ltr == "f") {
				Vec3f temp[3];
				int index;
				for (int a = 0; a < 3; a++) {
					stream >> index;
					stream.ignore(1000, ' ');
					temp[a] = pts[index - 1];
				}
				triangles.push_back(temp);
				
				// triangulate quad faces
				int index2;
				stream >> index2;
				if (index != index2) {
					Vec3f temp2[3] = { temp[2], pts[index2 - 1], temp[0] };
					triangles.push_back(temp2);
				}
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
