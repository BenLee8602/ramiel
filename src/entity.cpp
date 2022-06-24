#include "draw.h"
#include "triangle.h"
#include "objloader.h"
#include "graphics.h"

namespace ramiel {
	
	Entity::Entity(
		Mesh* mesh,
		float scale,
		Texture* texture,
		Texture* normalMap,
		ShadingType_ shading,
		Vec3f color,
		unsigned specularExponent,
		float specularIntensity,
		PhysicsObj* physicsObj
	) {
		this->shading = shading;
		c_clamp(color);
		this->color = color / 255.0f;
		this->physicsObj = physicsObj;
		this->texture = texture;
		this->normalMap = normalMap;
		this->mesh = mesh;
		this->scale = scale;
		this->specularExponent = specularExponent;
		this->specularIntensity = specularIntensity;
	}


	Entity::~Entity() {
		delete physicsObj;
	}


	void Entity::draw() {
		// get mesh info
		std::vector<Vec3f> v_pos, v_normal;
		mesh->getVPos(v_pos, scale, physicsObj->getPos(), physicsObj->getRot());
		mesh->getVNormal(v_normal, physicsObj->getRot());
		const std::vector<Vec2f>& v_txt   = mesh->getVTxt();
		const std::vector<Vec3u>& tri     = mesh->getTri();
		const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();

		// get camera coords
		std::vector<Vec3f> cameraCoords(v_pos.size());
		for (size_t i = 0; i < v_pos.size(); i++) {
			cameraCoords[i] = graphics::camera.getCameraCoord(v_pos[i]);
		}
		
		// graphics
		if (texture && tri_txt.size() == tri.size()) {
			switch (shading) {
				case ShadingType_::FLAT: {
					DrawFlat_Textured draw;
					draw.texture = texture;
					draw.specularExponent = specularExponent;
					draw.specularIntensity = specularIntensity;
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
					for (size_t i = 0; i < v_color.size(); ++i) {
						v_color[i] = graphics::getAllLights(
							v_pos[i], v_normal[i],
							specularExponent, specularIntensity
						);
					}
					DrawVertex_Textured draw;
					draw.texture = texture;
					draw.specularExponent = specularExponent;
					draw.specularIntensity = specularIntensity;
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
					if (normalMap) {
						DrawPixel_Textured_NormalMapped draw;
						draw.texture = texture;
						draw.normalMap = normalMap;
						draw.specularExponent = specularExponent;
						draw.specularIntensity = specularIntensity;
						for (size_t i = 0; i < tri.size(); ++i) {
							for (size_t j = 0; j < 3; ++j) {
								draw.v_pos[j] = v_pos[tri[i][j]];
								draw.tricam[j] = cameraCoords[tri[i][j]];
								draw.v_txt[j] = v_txt[tri_txt[i][j]];
							}
							triangle::draw(draw);
						}
					}
					else {
						DrawPixel_Textured draw;
						draw.texture = texture;
						draw.specularExponent = specularExponent;
						draw.specularIntensity = specularIntensity;
						for (size_t i = 0; i < tri.size(); ++i) {
							for (size_t j = 0; j < 3; ++j) {
								draw.v_pos[j] = v_pos[tri[i][j]];
								draw.tricam[j] = cameraCoords[tri[i][j]];
								draw.v_txt[j] = v_txt[tri_txt[i][j]];
							}
							triangle::draw(draw);
						}
					}
					break;
				}

				case ShadingType_::PIXEL_S: {
					DrawPixel_S_Textured draw;
					draw.texture = texture;
					draw.specularExponent = specularExponent;
					draw.specularIntensity = specularIntensity;
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
					draw.specularExponent = specularExponent;
					draw.specularIntensity = specularIntensity;
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
					for (size_t i = 0; i < v_color.size(); ++i) {
						v_color[i] = graphics::getAllLights(
							v_pos[i], v_normal[i],
							specularExponent, specularIntensity
						);
					}
					DrawVertex draw;
					draw.surfaceColor = color;
					draw.specularExponent = specularExponent;
					draw.specularIntensity = specularIntensity;
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
					if (normalMap && tri_txt.size() == tri.size()) {
						DrawPixel_NormalMapped draw;
						draw.surfaceColor = color;
						draw.normalMap = normalMap;
						draw.specularExponent = specularExponent;
						draw.specularIntensity = specularIntensity;
						for (size_t i = 0; i < tri.size(); ++i) {
							for (size_t j = 0; j < 3; ++j) {
								draw.v_pos[j] = v_pos[tri[i][j]];
								draw.tricam[j] = cameraCoords[tri[i][j]];
								draw.v_txt[j] = v_txt[tri_txt[i][j]];
							}
							triangle::draw(draw);
						}
					}
					else {
						DrawPixel draw;
						draw.surfaceColor = color;
						draw.specularExponent = specularExponent;
						draw.specularIntensity = specularIntensity;
						for (size_t i = 0; i < tri.size(); ++i) {
							for (size_t j = 0; j < 3; ++j) {
								draw.v_pos[j] = v_pos[tri[i][j]];
								draw.tricam[j] = cameraCoords[tri[i][j]];
							}
							triangle::draw(draw);
						}
					}
					
					break;
				}

				case ShadingType_::PIXEL_S: {
					DrawPixel_S draw;
					draw.surfaceColor = color;
					draw.specularExponent = specularExponent;
					draw.specularIntensity = specularIntensity;
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
