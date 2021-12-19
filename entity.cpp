#include <string>
#include <fstream>
#include <sstream>
#include "entity.h"
#include "render.h"

namespace bl {

	Entity::Entity(const char* filename, ShadingType shading, Vec3f pos, Vec3f color) {
		this->shading = shading;

		this->color = vec3f_255;
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
				stream >> temp.pos[X]>> temp.pos[Y] >> temp.pos[Z];
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
					
					// vertex normals
					Vec3f n = crossProduct(temp[1]->pos - temp[0]->pos, temp2[1]->pos - temp[0]->pos);
					for (int a = 0; a < 3; a++) temp[a]->normal += n;
					temp2[1]->normal += n;
				}
				else {
					// vertex normals
					Vec3f n = crossProduct(temp[1]->pos - temp[0]->pos, temp[2]->pos - temp[0]->pos);
					for (int a = 0; a < 3; a++) temp[a]->normal += n;
				}
			}
		}

		file.close();
		for (auto& v : vertices) v.normal = getNormalized(v.normal);
	}


	void Entity::calcVertexColor() {
		for (auto& v : vertices) {
			v.color = RenderBL::light_ambient;
			for (auto& l : RenderBL::lights) {
				l->getLight(v);
			}
			c_min(v.color);
			v.color -= color;
			c_max(v.color, RenderBL::light_ambient);
		}
	}


	void Entity::draw() {
		switch (shading) {
		case ShadingType::FLAT:
			for (auto& t : triangles) {
				t.draw_f(color);
			}
			break;
		case ShadingType::VERTEX:
			calcVertexColor();
			for (auto& t : triangles) {
				t.draw_v();
			}
			break;
		case ShadingType::PIXEL:
			for (auto& t : triangles) {
				t.draw_p(color);
			}
		}

		//for (auto& v : vertices) RenderBL::drawLine(v.pos, v.pos + v.normal / 20, { 0, 0, 255 });
	}

}
