#include <string>
#include <fstream>
#include <sstream>
#include "entity.h"
#include "render.h"

namespace bl {

	Entity::Entity(const char* filename, const Vec3f& pos) {
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
				Vertex temp;
				stream >> temp.pos.x >> temp.pos.y >> temp.pos.z;
				temp.pos += pos;
				vertices.push_back(temp);
			}

			// triangle data
			else if (ltr == "f") {
				Vertex* temp[3];
				int index;
				for (int a = 0; a < 3; a++) {
					stream >> index;
					stream.ignore(1000, ' ');
					temp[a] = &vertices[index - 1];
				}
				triangles.push_back(temp);
				
				// triangulate quad faces
				int index2 = index;
				stream >> index2;
				if (index != index2) {
					Vertex* temp2[3] = { temp[2], &vertices[index2 - 1], temp[0] };
					triangles.push_back(temp2);
				}
			}
		}
		file.close();
		calcVertexNormals();
	}


	void Entity::calcVertexNormals() {
		for (auto& v : vertices) {
			Vec3f normal = { 0.0f };
			int numVecs = 0;
			for (auto& t : triangles) {
				for (int a = 0; a < 3; a++) {
					if (t[a] == &v) {
						normal += t.getNormal();
						numVecs++;
						break;
					}
				}
			}
			v.normal = normal / (float)numVecs;
			RenderBL::debug << v.pos << " || " << v.normal << std::endl;
		}
	}
	

	void Entity::draw() const {
		for (auto& t : triangles) {
			t.draw();
		}
	}

}
