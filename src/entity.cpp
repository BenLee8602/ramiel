#include "draw.h"
#include "triangle.h"
#include "objloader.h"
#include "graphicsbl_p.h"

namespace bl {
	
	Entity::Entity(const char* filename, ShadingType_ shading, Vec3f color, Physics physics, Texture* texture) {
		this->shading = shading;
		c_clamp(color);
		this->color = color / 255.0f;
		this->physics = physics;
		this->texture = texture;

		// allocate memory
		size_t n_v, n_vt, n_f;
		objloader::count(filename, n_v, n_vt, n_f);
		v_pos.reserve(n_v);
		v_normal = std::vector<Vec3f>(n_v);
		tri.reserve(n_f);
		if (n_vt) {
			v_txt.reserve(n_vt);
			tri_txt.reserve(n_f);
		}
		
		// get obj data
		objloader::load(filename, v_pos, v_txt, tri, tri_txt);
		for (auto& v : v_pos) v += physics.pos;

		// calc vertex normals
		std::vector<Vec3f> tri_normals(tri.size());
		for (auto& t : tri) {
			Vec3f v1 = v_pos[t[1]] - v_pos[t[0]];
			Vec3f v2 = v_pos[t[2]] - v_pos[t[0]];
			Vec3f normal = crossProduct(v1, v2);
			for (int i = 0; i < 3; ++i) v_normal[t[i]] += normal;
		}
		for (auto& n : v_normal) n = getNormalized(n);
	}


	void Entity::calcVertexColor(std::vector<Vec3f>& v_color) {
		for (size_t i = 0; i < v_color.size(); ++i) {
			v_color[i] = GraphicsBL::light_ambient;
			for (auto& l : GraphicsBL::lights) {
				v_color[i] += l->getLight(v_pos[i], v_normal[i]);
			}
		}
	}


	void Entity::draw() {
		// physics
		if (physics.movement) {
			if (physics.velocity) {
				Vec3f dpos = physics.velocity * GraphicsBL::dtime;
				for (auto& v : v_pos) v += dpos;
			}
			physics.simulateMovement();
		}

		// get camera coords
		std::vector<Vec3f> cameraCoords(v_pos.size());
		for (size_t i = 0; i < v_pos.size(); i++) {
			cameraCoords[i] = GraphicsBL::camera.getCameraCoord(v_pos[i]);
		}
		
		// graphics
		if (texture && tri_txt.size() == tri.size()) {
			switch (shading) {
				case ShadingType_::FLAT: {
					DrawFlat_TX draw;
					draw.texture = texture;
					for (size_t i = 0; i < tri.size(); ++i) {
						for (size_t j = 0; j < 3; ++j) {
							draw.v_pos[j] = v_pos[tri[i][j]];
							draw.tricam[j] = cameraCoords[tri[i][j]];
							draw.v_txt[j] = v_txt[tri_txt[i][j]];
						}
						triangle::draw(draw);
					}
					break;
				}

				case ShadingType_::VERTEX: {
					std::vector<Vec3f> v_color(v_pos.size());
					calcVertexColor(v_color);
					DrawVertex_TX draw;
					draw.texture = texture;
					for (size_t i = 0; i < tri.size(); ++i) {
						for (size_t j = 0; j < 3; ++j) {
							draw.v_pos[j] = v_pos[tri[i][j]];
							draw.v_color[j] = v_color[tri[i][j]];
							draw.tricam[j] = cameraCoords[tri[i][j]];
							draw.v_txt[j] = v_txt[tri_txt[i][j]];
						}
						triangle::draw(draw);
					}
					break;
				}

				case ShadingType_::PIXEL: {
					DrawPixel_TX draw;
					draw.texture = texture;
					for (size_t i = 0; i < tri.size(); ++i) {
						for (size_t j = 0; j < 3; ++j) {
							draw.v_pos[j] = v_pos[tri[i][j]];
							draw.tricam[j] = cameraCoords[tri[i][j]];
							draw.v_txt[j] = v_txt[tri_txt[i][j]];
						}
						triangle::draw(draw);
					}
					break;
				}

				case ShadingType_::PIXEL_S: {
					DrawPixel_S_TX draw;
					draw.texture = texture;
					for (size_t i = 0; i < tri.size(); ++i) {
						for (size_t j = 0; j < 3; ++j) {
							draw.v_pos[j] = v_pos[tri[i][j]];
							draw.v_normal[j] = v_normal[tri[i][j]];
							draw.tricam[j] = cameraCoords[tri[i][j]];
							draw.v_txt[j] = v_txt[tri_txt[i][j]];
						}
						triangle::draw(draw);
					}
					break;
				}

				default: throw std::string("Error in determining shading type");
			}
		}
		else {
			switch (shading) {
				case ShadingType_::FLAT: {
					DrawFlat draw;
					draw.surfaceColor = color;
					for (size_t i = 0; i < tri.size(); ++i) {
						for (size_t j = 0; j < 3; ++j) {
							draw.v_pos[j] = v_pos[tri[i][j]];
							draw.tricam[j] = cameraCoords[tri[i][j]];
						}
						triangle::draw(draw);
					}
					break;
				}

				case ShadingType_::VERTEX: {
					std::vector<Vec3f> v_color(v_pos.size());
					calcVertexColor(v_color);
					DrawVertex draw;
					draw.surfaceColor = color;
					for (size_t i = 0; i < tri.size(); ++i) {
						for (size_t j = 0; j < 3; ++j) {
							draw.v_pos[j] = v_pos[tri[i][j]];
							draw.v_color[j] = v_color[tri[i][j]];
							draw.tricam[j] = cameraCoords[tri[i][j]];
						}
						triangle::draw(draw);
					}
					break;
				}

				case ShadingType_::PIXEL: {
					DrawPixel draw;
					draw.surfaceColor = color;
					for (size_t i = 0; i < tri.size(); ++i) {
						for (size_t j = 0; j < 3; ++j) {
							draw.v_pos[j] = v_pos[tri[i][j]];
							draw.tricam[j] = cameraCoords[tri[i][j]];
						}
						triangle::draw(draw);
					}
					break;
				}

				case ShadingType_::PIXEL_S: {
					DrawPixel_S draw;
					draw.surfaceColor = color;
					for (size_t i = 0; i < tri.size(); ++i) {
						for (size_t j = 0; j < 3; ++j) {
							draw.v_pos[j] = v_pos[tri[i][j]];
							draw.v_normal[j] = v_normal[tri[i][j]];
							draw.tricam[j] = cameraCoords[tri[i][j]];
						}
						triangle::draw(draw);
					}
					break;
				}

				default: throw std::string("Error in determining shading type");
			}
		}
	}

}
