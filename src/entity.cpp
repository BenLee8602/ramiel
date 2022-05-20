#include "draw.h"
#include "triangle.h"
#include "objloader.h"
#include "graphicsbl_p.h"

namespace bl {
	
	Entity::Entity(Model* model, Texture* texture, ShadingType_ shading, Vec3f color, Physics physics) {
		this->shading = shading;
		c_clamp(color);
		this->color = color / 255.0f;
		this->physics = physics;
		this->texture = texture;
		this->model = model;
	}


	void Entity::calcVertexColor(
		std::vector<Vec3f>& v_color,
		const std::vector<Vec3f>& v_pos,
		const std::vector<Vec3f> v_normal
	) {
		for (size_t i = 0; i < v_color.size(); ++i) {
			v_color[i] = GraphicsBL::light_ambient;
			for (auto& l : GraphicsBL::lights) {
				v_color[i] += l->getLight(v_pos[i], v_normal[i]);
			}
		}
	}


	void Entity::draw() {
		// get model info
		std::vector<Vec3f> v_pos, v_normal;
		model->getVPos(v_pos, physics.pos, physics.rot);
		model->getVNormal(v_normal, physics.rot);
		const std::vector<Vec2f>& v_txt   = model->getVTxt();
		const std::vector<Vec3u>& tri     = model->getTri();
		const std::vector<Vec3u>& tri_txt = model->getTriTxt();

		// physics
		if (physics.movement) physics.simulateMovement();

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
					calcVertexColor(v_color, v_pos, v_normal);
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
					calcVertexColor(v_color, v_pos, v_normal);
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
