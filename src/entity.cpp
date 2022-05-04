#include <string>
#include <fstream>
#include <sstream>
#include "draw.h"
#include "graphicsbl_p.h"
using GraphicsBL = bl::GraphicsBL_p;

namespace bl {

	size_t Entity::totalVerts = 0;
	size_t Entity::totalTris = 0;

	size_t Entity::getTotalVerts() { return totalVerts; }
	size_t Entity::getTotalTris()  { return totalTris; }
	
	Entity::Entity(const char* filename, ShadingType_ shading, Vec3f color, Physics physics) {
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

		v_pos.reserve(nverts);
		//if (shading == ShadingType_::VERTEX || shading == ShadingType_::PIXEL_S) {
			v_normal = std::vector<Vec3f>(nverts);
		//}
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
				Vec3f temp;
				stream >> temp[X] >> temp[Y] >> temp[Z];
				temp += physics.pos;
				v_pos.push_back(temp);
			}

			// triangle data
			else if (ltr == "f") {
				size_t temp[3];
				size_t index;
				for (int i = 0; i < 3; ++i) {
					stream >> index;
					stream.ignore(1000, ' ');
					temp[i] = index - 1;
				}
				triangles.push_back(temp);

				// triangulate quad faces
				size_t index2 = index;
				stream >> index2;
				if (index != index2) {
					size_t temp2[3] = { temp[2], index2 - 1, temp[0] };
					triangles.push_back(temp2);

					// vertex normals
					Vec3f v1 = v_pos[temp[1]]  - v_pos[temp[0]];
					Vec3f v2 = v_pos[temp2[1]] - v_pos[temp[0]];
					Vec3f n = crossProduct(v1, v2);
					for (int i = 0; i < 3; ++i) v_normal[temp[i]] += n;
					v_normal[temp2[1]] += n;
				}
				else {
					// vertex normals
					Vec3f v1 = v_pos[temp[1]] - v_pos[temp[0]];
					Vec3f v2 = v_pos[temp[2]] - v_pos[temp[0]];
					Vec3f n = crossProduct(v1, v2);
					for (int i = 0; i < 3; ++i) v_normal[temp[i]] += n;
				}
			}
		}
		
		file.close();
		for (auto& v : v_normal) v = getNormalized(v);
	}


	void Entity::calcVertexColor(std::vector<Vec3f>& v_color) {
		for (size_t i = 0; i < v_color.size(); ++i) {
			v_color[i] = GraphicsBL::light_ambient;
			for (auto& l : GraphicsBL::lights) {
				v_color[i] += l->getLight(v_pos[i], v_normal[i]);
			}
			v_color[i] *= color; // rember to change this later
		}
	}


	void Entity::draw() {
		// physics
		if (physics.movement) {
			if (physics.velocity) {
				Vec3f dpos = physics.velocity * GraphicsBL::dtime;
				for (auto& v : v_pos) {
					v += dpos;
				}
			}
			physics.simulateMovement();
		}

		// get camera coords
		std::vector<Vec3f> cameraCoords(v_pos.size());
		for (size_t i = 0; i < v_pos.size(); i++) {
			cameraCoords[i] = GraphicsBL::camera.getCameraCoord(v_pos[i]);
		}
		
		// graphics
		switch (shading) {
			case ShadingType_::FLAT: {
				DrawFlat draw;
				draw.surfaceColor = color;
				for (auto& t : triangles) {
					for (size_t i = 0; i < 3; i++) {
						draw.v_pos[i] = v_pos[t[i]];
						draw.tricam[i] = cameraCoords[t[i]];
					}
					t.draw<GraphicsBL>(draw);
				}
				break;
			}

			case ShadingType_::VERTEX: {
				std::vector<Vec3f> v_color(v_pos.size());
				calcVertexColor(v_color);
				DrawVertex draw;
				draw.surfaceColor = color;
				for (auto& t : triangles) {
					for (size_t i = 0; i < 3; i++) {
						draw.v_pos[i] = v_pos[t[i]];
						draw.v_color[i] = v_color[t[i]];
						draw.tricam[i] = cameraCoords[t[i]];
					}
					t.draw<GraphicsBL>(draw);
				}
				break;
			}

			case ShadingType_::PIXEL: {
				DrawPixel draw;
				draw.surfaceColor = color;
				for (auto& t : triangles) {
					for (size_t i = 0; i < 3; i++) {
						draw.v_pos[i] = v_pos[t[i]];
						draw.tricam[i] = cameraCoords[t[i]];
					}
					t.draw<GraphicsBL>(draw);
				}
				break;
			}

			case ShadingType_::PIXEL_S: {
				DrawPixel_S draw;
				draw.surfaceColor = color;
				for (auto& t : triangles) {
					for (size_t i = 0; i < 3; i++) {
						draw.v_pos[i] = v_pos[t[i]];
						draw.v_normal[i] = v_normal[t[i]];
						draw.tricam[i] = cameraCoords[t[i]];
					}
					t.draw<GraphicsBL>(draw);
				}
				break;
			}

			default: throw std::string("Error in determining shading type");
		}
	}

}
