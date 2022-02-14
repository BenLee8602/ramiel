#include <string>
#include <fstream>
#include <sstream>
#include "graphics.h"

namespace bl {

	size_t Entity::totalVerts = 0;
	size_t Entity::totalTris = 0;

	size_t Entity::getTotalVerts() { return totalVerts; }
	size_t Entity::getTotalTris()  { return totalTris; }
	
	Entity::Entity(const char* filename, ShadingType shading, Vec3f color, Physics physics) {
		this->shading = shading;

		c_clamp(color);
		this->color = color / 255.0f;

		this->physics = physics;

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
				temp.pos += physics.pos;
				temp.color = this->color;
				vertices.push_back(temp);
			}

			// triangle data
			else if (ltr == "f") {
				size_t temp[3];
				size_t index;
				for (int a = 0; a < 3; a++) {
					stream >> index;
					stream.ignore(1000, ' ');
					temp[a] = index - 1;
				}
				triangles.push_back(temp);

				// triangulate quad faces
				size_t index2 = index;
				stream >> index2;
				if (index != index2) {
					size_t temp2[3] = { temp[2], index2 - 1, temp[0] };
					triangles.push_back(temp2);

					// vertex normals
					Vec3f v1 = vertices[temp[1]].pos - vertices[temp[0]].pos;
					Vec3f v2 = vertices[temp2[1]].pos - vertices[temp[0]].pos;
					Vec3f n = crossProduct(v1, v2);
					for (int a = 0; a < 3; a++) vertices[temp[a]].normal += n;
					vertices[temp2[1]].normal += n;
				}
				else {
					// vertex normals
					Vec3f v1 = vertices[temp[1]].pos - vertices[temp[0]].pos;
					Vec3f v2 = vertices[temp[2]].pos - vertices[temp[0]].pos;
					Vec3f n = crossProduct(v1, v2);
					for (int a = 0; a < 3; a++) vertices[temp[a]].normal += n;
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
		// physics
		if (physics.movement) {
			if (physics.velocity) {
				Vec3f dpos = physics.velocity * GraphicsBL::dtime;
				for (auto& v : vertices) {
					v.pos += dpos;
				}
			}
			physics.simulateMovement();
		}
		
		// camera coords
		std::vector<Vec3f> cameraCoords(vertices.size());
		for (size_t i = 0; i < vertices.size(); i++) {
			cameraCoords[i] = GraphicsBL::camera.getCameraCoord(vertices[i].pos);
		}

		// graphics
		switch (shading) {
		case ShadingType::FLAT:
			for (auto& t : triangles) {
				Vertex v[3] = { vertices[t[0]], vertices[t[1]], vertices[t[2]] };
				Vec3f tricam[3] = { cameraCoords[t[0]], cameraCoords[t[1]], cameraCoords[t[2]] };
				t.draw_f(v, tricam);
			}
			break;
		case ShadingType::VERTEX:
			calcVertexColor();
			for (auto& t : triangles) {
				Vertex v[3] = { vertices[t[0]], vertices[t[1]], vertices[t[2]] };
				Vec3f tricam[3] = { cameraCoords[t[0]], cameraCoords[t[1]], cameraCoords[t[2]] };
				t.draw_v(v, tricam);
			}
			break;
		case ShadingType::PIXEL:
			for (auto& t : triangles) {
				Vertex v[3] = { vertices[t[0]], vertices[t[1]], vertices[t[2]] };
				Vec3f tricam[3] = { cameraCoords[t[0]], cameraCoords[t[1]], cameraCoords[t[2]] };
				t.draw_p(v, tricam);
			}
		}
	}

}
