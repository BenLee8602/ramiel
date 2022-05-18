#include <thread>
#include <fstream>
#include "graphicsbl_p.h"
#include "../include/graphicsbl.h"

namespace bl {

	float  GraphicsBL::dtime = 0.0f;
	Camera GraphicsBL::camera;
	Bloom  GraphicsBL::bloom(50);

	Vec2u  GraphicsBL::size = { 0 };
	Vec2u  GraphicsBL::mid = { 0 };
	size_t GraphicsBL::bufferSize = 0;

	std::vector<Vec3f> GraphicsBL::pixels;
	std::vector<float> GraphicsBL::depth;

	std::unordered_map<std::string, Texture*> GraphicsBL::textures;

	std::vector<Entity*> GraphicsBL::entities;
	std::vector<Light*>  GraphicsBL::lights;
	std::vector<Effect*> GraphicsBL::effects;
	Vec3f GraphicsBL::light_ambient = vec3f_0;
	Vec3f GraphicsBL::bg_color = vec3f_0;


	void GraphicsBL::setBufferSize(Vec2u size) {
		GraphicsBL::size = size;
		mid = size / 2;
		bufferSize = size[X] * size[Y];
		pixels = std::vector<Vec3f>(bufferSize);
		depth = std::vector<float>(bufferSize);
		setFov(90U);
	}


	void GraphicsBL::setFov(unsigned fov) {
		camera.setFov(fov);
	}


	void GraphicsBL::setControls(bool controls[12]) {
		camera.setControls(controls);
	}

	
	void GraphicsBL::setAmbientLightColor(Vec3f color) {
		c_max(color);
		light_ambient = color;
	}

	
	void GraphicsBL::setBackgroundColor(Vec3f color) {
		c_max(color);
		bg_color = color;
	}

	
	void GraphicsBL::renderFrame(float dtime) {
		std::fill(pixels.begin(), pixels.end(), bg_color);
		std::fill(depth.begin(), depth.end(), camera.zfar);
		
		GraphicsBL::dtime = dtime;
		camera.calcTrigValues();
		
		drawEntities();
		getCollisions();

		for (auto& e : effects) e->applyEffect(&pixels[0], &pixels[0]);
		bloom.applyEffect(&pixels[0], &pixels[0]);
		for (size_t i = 0; i < bufferSize; i++) notBloom(pixels[i]);
	}


	void GraphicsBL::drawEntities() {
		const size_t nthreads = std::min<size_t>(entities.size(), std::thread::hardware_concurrency());
		const size_t ePerThread = entities.size() / nthreads;
		std::thread* threads = new std::thread[nthreads];

		auto drawPortion = [](size_t begin, size_t end) {
			for (size_t i = begin; i < end; i++) {
				entities[i]->draw();
			}
		};

		size_t begin = 0;
		for (size_t i = 0; i < nthreads - 1; i++) {
			threads[i] = std::thread(drawPortion, begin, begin + ePerThread);
			begin += ePerThread;
		}
		threads[nthreads - 1] = std::thread(drawPortion, begin, entities.size());
		for (size_t i = 0; i < nthreads; i++) {
			threads[i].join();
		}

		delete[] threads;
	}


	void GraphicsBL::getCollisions() {
		for (size_t a = 0; a < entities.size() - 1; a++) {
			if (entities[a]->physics.collision) {
				for (size_t b = a + 1; b < entities.size(); b++) {
					if (entities[b]->physics.collision) {
						entities[a]->physics.simulateCollision(entities[b]->physics);
					}
				}
			}
		}
	}

	
	void GraphicsBL::getFrameDEC(int* frame) {
		for (size_t i = 0; i < bufferSize; ++i) {
			frame[i] = rgbToDec(pixels[i]);
		}
	}

	
	void GraphicsBL::getFrameRGB(uint8_t* frame) {
		uint8_t* f = frame;
		for (size_t i = 0; i < bufferSize; ++i) {
			*f++ = pixels[i][R];
			*f++ = pixels[i][G];
			*f++ = pixels[i][B];
		}
	}


	bool GraphicsBL::loadTexture(const char* name, const char* filename) {
		if (!std::ifstream(filename).good()) return false;
		textures[std::string(name)] = new Texture(filename);
		return true;
	}


	ShadingType_ mapShadingType(ShadingType shading) {
		switch (shading) {
			case ShadingType::FLAT:    return ShadingType_::FLAT;
			case ShadingType::VERTEX:  return ShadingType_::VERTEX;
			case ShadingType::PIXEL:   return ShadingType_::PIXEL;
			case ShadingType::PIXEL_S: return ShadingType_::PIXEL_S;
		}
		return ShadingType_::FLAT;
	}

	
	bool GraphicsBL::addEntity(
		const char* filename, ShadingType shading, Vec3f color, Vec3f pos,
		bool collision, float hbxrad, float mass,
		bool movement, Vec3f velocity, Vec3f acceleration
	) {
		if (!std::ifstream(filename).good()) return false;
		entities.push_back(new Entity(
			filename,
			mapShadingType(shading),
			color,
			Physics(
				pos,
				collision,
				hbxrad,
				mass,
				movement,
				velocity,
				acceleration
			)
		));
		return true;
	}


	bool GraphicsBL::addEntity(
		const char* model, const char* texture, ShadingType shading, Vec3f pos,
		bool collision, float hbxrad, float mass,
		bool movement, Vec3f velocity, Vec3f acceleration
	) {
		if (!std::ifstream(model).good()) return false;
		entities.push_back(new Entity(
			model,
			mapShadingType(shading),
			vec3f_255,
			Physics(
				pos,
				collision,
				hbxrad,
				mass,
				movement,
				velocity,
				acceleration
			),
			textures[texture]
		));
		return true;
	}

	
	void GraphicsBL::addDirLight(
		Vec3f color,
		Vec3f dir
	) { lights.push_back(new Light_Dir(color, dir)); }


	void GraphicsBL::addPointLight(
		Vec3f color,
		Vec3f pos,
		float falloff
	) { lights.push_back(new Light_Pt(color, pos, falloff)); }


	void GraphicsBL::addSpotLight(
		Vec3f color, Vec3f pos, Vec3f dir,
		float falloff, float width, float falloffExp
	) {
		lights.push_back(new Light_Sp(color, pos, dir, falloff, width, falloffExp));
	}

	
	void GraphicsBL::addEffect(Effect* effect) { // temp
		effects.push_back(effect);
	}

	
	void GraphicsBL::removeEntity(size_t index) {
		if (entities.size()) {
			index %= entities.size();
			entities.erase(entities.begin() + index);
		}
	}

	
	void GraphicsBL::removeLight(size_t index) {
		if (lights.size()) {
			index %= lights.size();
			lights.erase(lights.begin() + index);
		}
	}


	void GraphicsBL::removeEffect(size_t index) {
		if (effects.size()) {
			index %= effects.size();
			effects.erase(effects.begin() + index);
		}
	}


	int GraphicsBL::coordsToIndex(const Vec2& in) {
		return size[X] * in[Y] + in[X];
	}

}
