#include <string>
#include <fstream>
#include <sstream>
#include "entity.h"
#include "render.h"

namespace bl {

	Entity::Entity(const char* filename, Vec3f& pos, Vec3f& color, char shading) {
		this->shading = shading;
		if (shading != 'f' && shading != 'v') {
			this->shading = 'f';
		}

		this->color = 255.0f;
		this->color -= color;
		c_clamp(this->color);

		std::ifstream file(filename);
		std::string line;

		// get vertex data
		while (getline(file, line)) {
			// insert line content into stream
			std::stringstream stream;
			stream << line;

			// determine type of data on current line
			std::string ltr;
			stream >> ltr;

			if (ltr == "v") {
				Vertex temp;
				stream >> temp.pos[x]>> temp.pos[y] >> temp.pos[z];
				temp.pos += pos;
				vertices.push_back(temp);
			}
		}

		file.close();
		file.open(filename);

		// get triangle data
		while (getline(file, line)) {
			// insert line content into stream
			std::stringstream stream;
			stream << line;

			// determine type of data on current line
			std::string ltr;
			stream >> ltr;

			if (ltr == "f") {
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

		calcVertexNormals();
	}


	void Entity::calcVertexNormals() {
		for (auto& t : triangles) {
			for (int a = 0; a < 3; a++) {
				t[a]->normal += t.getNormal();
			}
		}
		for (auto& v : vertices) {
			v.normal = getNormalized(v.normal);
		}
	}


	void Entity::calcVertexColor() {
		for (auto& v : vertices) {
			v.color = RenderBL::light_ambient;
			for (auto& l : RenderBL::lights) {
				l->getLight(v);
			}
			c_min(v.color);
			v.color -= color;
			c_max(v.color);
		}
	}


	void Entity::draw() {
		switch (shading) {
		case 'f':
			for (auto& t : triangles) {
				t.draw_f(color);
			}
			break;
		case 'v':
			calcVertexColor();
			for (auto& t : triangles) {
				t.draw_v();
			}
			break;
		}
	}

}
