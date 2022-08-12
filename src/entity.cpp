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

		bool validTexture = texture && mesh->getTri().size() == mesh->getTriTxt().size();
		switch (shading) {
			case ShadingType_::FLAT: this->draw = validTexture ? &draw_flat_textured : &draw_flat; break;
			case ShadingType_::VERTEX: this->draw = validTexture ? &draw_vertex_textured : &draw_vertex; break;
			case ShadingType_::PIXEL:
				if (normalMap) this->draw = validTexture ? &draw_pixel_textured_normalmapped : &draw_pixel_normalmapped;
				else this->draw = validTexture ? &draw_pixel_textured : &draw_pixel;
				break;
			case ShadingType_::PIXEL_S: this->draw = validTexture ? &draw_pixel_smooth_textured : &draw_pixel_smooth; break;
		}
	}


	Entity::~Entity() {
		delete physicsObj;
	}


	void Entity::draw_flat() {
		// get mesh info
		std::vector<Vec3f> v_pos;
		mesh->getVPos(v_pos, scale, physicsObj->getPos(), physicsObj->getRot());
		const std::vector<Vec3u>& tri = mesh->getTri();

		// get camera coords
		std::vector<Vec3f> cameraCoords(v_pos.size());
		for (size_t i = 0; i < v_pos.size(); i++) {
			cameraCoords[i] = graphics::camera.getCameraCoord(v_pos[i]);
		}
		
		// create draw object
		DrawFlat draw;
		draw.surfaceColor = color;
		draw.specularExponent = specularExponent;
		draw.specularIntensity = specularIntensity;

		// draw each tri
		for (size_t i = 0; i < tri.size(); ++i) {
			for (size_t j = 0; j < 3; ++j) {
				draw.v_pos[j] = v_pos[tri[i][j]];
				draw.tricam[j] = cameraCoords[tri[i][j]];
			}
			triangle::draw(draw);
		}
	}


	void Entity::draw_flat_textured() {
		// get mesh info
		std::vector<Vec3f> v_pos;
		mesh->getVPos(v_pos, scale, physicsObj->getPos(), physicsObj->getRot());
		const std::vector<Vec2f>& v_txt   = mesh->getVTxt();
		const std::vector<Vec3u>& tri     = mesh->getTri();
		const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();

		// get camera coords
		std::vector<Vec3f> cameraCoords(v_pos.size());
		for (size_t i = 0; i < v_pos.size(); i++) {
			cameraCoords[i] = graphics::camera.getCameraCoord(v_pos[i]);
		}
		
		// create draw object
		DrawFlat_Textured draw;
		draw.texture = texture;
		draw.specularExponent = specularExponent;
		draw.specularIntensity = specularIntensity;

		// draw each tri
		for (size_t i = 0; i < tri.size(); ++i) {
			for (size_t j = 0; j < 3; ++j) {
				draw.v_pos[j] = v_pos[tri[i][j]];
				draw.tricam[j] = cameraCoords[tri[i][j]];
				draw.v_txt[j] = v_txt[tri_txt[i][j]];
			}
			triangle::draw(draw);
		}
	}


	void Entity::draw_vertex() {
		// get mesh info
		std::vector<Vec3f> v_pos, v_normal;
		mesh->getVPos(v_pos, scale, physicsObj->getPos(), physicsObj->getRot());
		mesh->getVNormal(v_normal, physicsObj->getRot());
		const std::vector<Vec3u>& tri = mesh->getTri();

		// get camera coords
		std::vector<Vec3f> cameraCoords(v_pos.size());
		for (size_t i = 0; i < v_pos.size(); i++) {
			cameraCoords[i] = graphics::camera.getCameraCoord(v_pos[i]);
		}
		
		// get vertex colors
		std::vector<Vec3f> v_color(v_pos.size());
		for (size_t i = 0; i < v_color.size(); ++i) {
			v_color[i] = graphics::getAllLights(
				v_pos[i], v_normal[i],
				specularExponent, specularIntensity
			);
		}

		// create draw object
		DrawVertex draw;
		draw.surfaceColor = color;
		draw.specularExponent = specularExponent;
		draw.specularIntensity = specularIntensity;

		// draw each tri
		for (size_t i = 0; i < tri.size(); ++i) {
			for (size_t j = 0; j < 3; ++j) {
				draw.v_pos[j] = v_pos[tri[i][j]];
				draw.v_color[j] = v_color[tri[i][j]];
				draw.tricam[j] = cameraCoords[tri[i][j]];
			}
			triangle::draw(draw);
		}
	}


	void Entity::draw_vertex_textured() {
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
		
		// get vertex colors
		std::vector<Vec3f> v_color(v_pos.size());
		for (size_t i = 0; i < v_color.size(); ++i) {
			v_color[i] = graphics::getAllLights(
				v_pos[i], v_normal[i],
				specularExponent, specularIntensity
			);
		}

		// create draw object
		DrawVertex_Textured draw;
		draw.texture = texture;
		draw.specularExponent = specularExponent;
		draw.specularIntensity = specularIntensity;

		// draw each tri
		for (size_t i = 0; i < tri.size(); ++i) {
			for (size_t j = 0; j < 3; ++j) {
				draw.v_pos[j] = v_pos[tri[i][j]];
				draw.v_color[j] = v_color[tri[i][j]];
				draw.tricam[j] = cameraCoords[tri[i][j]];
				draw.v_txt[j] = v_txt[tri_txt[i][j]];
			}
			triangle::draw(draw);
		}
	}


	void Entity::draw_pixel() {
		// get mesh info
		std::vector<Vec3f> v_pos;
		mesh->getVPos(v_pos, scale, physicsObj->getPos(), physicsObj->getRot());
		const std::vector<Vec3u>& tri = mesh->getTri();

		// get camera coords
		std::vector<Vec3f> cameraCoords(v_pos.size());
		for (size_t i = 0; i < v_pos.size(); i++) {
			cameraCoords[i] = graphics::camera.getCameraCoord(v_pos[i]);
		}
		
		// create draw object
		DrawPixel draw;
		draw.surfaceColor = color;
		draw.specularExponent = specularExponent;
		draw.specularIntensity = specularIntensity;

		// draw each tri
		for (size_t i = 0; i < tri.size(); ++i) {
			for (size_t j = 0; j < 3; ++j) {
				draw.v_pos[j] = v_pos[tri[i][j]];
				draw.tricam[j] = cameraCoords[tri[i][j]];
			}
			triangle::draw(draw);
		}
	}


	void Entity::draw_pixel_textured() {
		// get mesh info
		std::vector<Vec3f> v_pos;
		mesh->getVPos(v_pos, scale, physicsObj->getPos(), physicsObj->getRot());
		const std::vector<Vec2f>& v_txt   = mesh->getVTxt();
		const std::vector<Vec3u>& tri     = mesh->getTri();
		const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();

		// get camera coords
		std::vector<Vec3f> cameraCoords(v_pos.size());
		for (size_t i = 0; i < v_pos.size(); i++) {
			cameraCoords[i] = graphics::camera.getCameraCoord(v_pos[i]);
		}
		
		// create draw object
		DrawPixel_Textured draw;
		draw.texture = texture;
		draw.specularExponent = specularExponent;
		draw.specularIntensity = specularIntensity;

		// draw each tri
		for (size_t i = 0; i < tri.size(); ++i) {
			for (size_t j = 0; j < 3; ++j) {
				draw.v_pos[j] = v_pos[tri[i][j]];
				draw.tricam[j] = cameraCoords[tri[i][j]];
				draw.v_txt[j] = v_txt[tri_txt[i][j]];
			}
			triangle::draw(draw);
		}
	}


	void Entity::draw_pixel_normalmapped() {
		// get mesh info
		std::vector<Vec3f> v_pos;
		mesh->getVPos(v_pos, scale, physicsObj->getPos(), physicsObj->getRot());
		const std::vector<Vec2f>& v_txt   = mesh->getVTxt();
		const std::vector<Vec3u>& tri     = mesh->getTri();
		const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();

		// get camera coords
		std::vector<Vec3f> cameraCoords(v_pos.size());
		for (size_t i = 0; i < v_pos.size(); i++) {
			cameraCoords[i] = graphics::camera.getCameraCoord(v_pos[i]);
		}
		
		// create draw object
		DrawPixel_NormalMapped draw;
		draw.surfaceColor = color;
		draw.normalMap = normalMap;
		draw.specularExponent = specularExponent;
		draw.specularIntensity = specularIntensity;

		// draw each tri
		for (size_t i = 0; i < tri.size(); ++i) {
			for (size_t j = 0; j < 3; ++j) {
				draw.v_pos[j] = v_pos[tri[i][j]];
				draw.tricam[j] = cameraCoords[tri[i][j]];
				draw.v_txt[j] = v_txt[tri_txt[i][j]];
			}
			triangle::draw(draw);
		}
	}


	void Entity::draw_pixel_textured_normalmapped() {
		// get mesh info
		std::vector<Vec3f> v_pos;
		mesh->getVPos(v_pos, scale, physicsObj->getPos(), physicsObj->getRot());
		const std::vector<Vec2f>& v_txt   = mesh->getVTxt();
		const std::vector<Vec3u>& tri     = mesh->getTri();
		const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();

		// get camera coords
		std::vector<Vec3f> cameraCoords(v_pos.size());
		for (size_t i = 0; i < v_pos.size(); i++) {
			cameraCoords[i] = graphics::camera.getCameraCoord(v_pos[i]);
		}
		
		// create draw object
		DrawPixel_Textured_NormalMapped draw;
		draw.texture = texture;
		draw.normalMap = normalMap;
		draw.specularExponent = specularExponent;
		draw.specularIntensity = specularIntensity;

		// draw each tri
		for (size_t i = 0; i < tri.size(); ++i) {
			for (size_t j = 0; j < 3; ++j) {
				draw.v_pos[j] = v_pos[tri[i][j]];
				draw.tricam[j] = cameraCoords[tri[i][j]];
				draw.v_txt[j] = v_txt[tri_txt[i][j]];
			}
			triangle::draw(draw);
		}
	}


	void Entity::draw_pixel_smooth() {
		// get mesh info
		std::vector<Vec3f> v_pos, v_normal;
		mesh->getVPos(v_pos, scale, physicsObj->getPos(), physicsObj->getRot());
		mesh->getVNormal(v_normal, physicsObj->getRot());
		const std::vector<Vec3u>& tri     = mesh->getTri();

		// get camera coords
		std::vector<Vec3f> cameraCoords(v_pos.size());
		for (size_t i = 0; i < v_pos.size(); i++) {
			cameraCoords[i] = graphics::camera.getCameraCoord(v_pos[i]);
		}
		
		// create draw object
		DrawPixel_S draw;
		draw.surfaceColor = color;
		draw.specularExponent = specularExponent;
		draw.specularIntensity = specularIntensity;

		// draw each tri
		for (size_t i = 0; i < tri.size(); ++i) {
			for (size_t j = 0; j < 3; ++j) {
				draw.v_pos[j] = v_pos[tri[i][j]];
				draw.v_normal[j] = v_normal[tri[i][j]];
				draw.tricam[j] = cameraCoords[tri[i][j]];
			}
			triangle::draw(draw);
		}
	}


	void Entity::draw_pixel_smooth_textured() {
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
		
		// create draw object
		DrawPixel_S_Textured draw;
		draw.texture = texture;
		draw.specularExponent = specularExponent;
		draw.specularIntensity = specularIntensity;

		// draw each tri
		for (size_t i = 0; i < tri.size(); ++i) {
			for (size_t j = 0; j < 3; ++j) {
				draw.v_pos[j] = v_pos[tri[i][j]];
				draw.v_normal[j] = v_normal[tri[i][j]];
				draw.tricam[j] = cameraCoords[tri[i][j]];
				draw.v_txt[j] = v_txt[tri_txt[i][j]];
			}
			triangle::draw(draw);
		}
	}

}
