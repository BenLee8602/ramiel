#include <string>
#include <fstream>
#include <sstream>
#include "graphics.h"

namespace bl {

	size_t Entity::totalVerts = 0;
	size_t Entity::totalTris = 0;
	
	Entity::Entity(const char* filename, ShadingType shading, Vec3f pos, Vec3f color) {
		this->shading = shading;

		c_clamp(color);
		this->color = color / 255.0f;

		std::ifstream file(filename);
		std::string line;

		// allocate memory
		size_t nverts = 0;
		size_t ntris = 0;

		while (std::getline(file, line)) {
			if (line[0] == 'v') ++nverts;

			else if (line[0] == 'f') {
				++ntris;
				int v3 = 0;
				int v4 = 0;
				std::stringstream stream(line);
				stream >> v3 >> v3 >> v3 >> v4;
				if (v3 == v4) ++ntris;
			}
		}

		totalVerts += nverts;
		totalTris += ntris;
		vertices.reserve(nverts);
		triangles.reserve(ntris);

		file.close();
		file.open(filename);

		// get vertex and triangle data
		while (std::getline(file, line)) {
			// insert line content into stream
			std::stringstream stream;
			stream << line;

			// determine type of data on current line
			std::string ltr;
			stream >> ltr;

			// vertex data
			if (ltr == "v") {
				Vertex temp;
				stream >> temp.pos[X] >> temp.pos[Y] >> temp.pos[Z];
				temp.pos += pos;
				vertices.push_back(temp);
			}

			// triangle data
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
			v.color = GraphicsBL::light_ambient;
			for (auto& l : GraphicsBL::lights) {
				l->getLight(v);
			}
			v.color *= color;
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
