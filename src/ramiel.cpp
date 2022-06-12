#include <thread>
#include <fstream>
#include "ramiel_p.h"
#include "physics.h"
#include "../include/ramiel.h"

namespace ramiel {

	float  graphics::dtime = 0.0f;
	Camera graphics::camera;
	Bloom  graphics::bloom(50);

	Vec2u  graphics::size = { 0 };
	Vec2u  graphics::mid = { 0 };
	size_t graphics::bufferSize = 0;

	std::vector<Vec3f> graphics::pixels;
	std::vector<float> graphics::depth;

	std::unordered_map<std::string, Model*>   graphics::models;
	std::unordered_map<std::string, Texture*> graphics::textures;

	std::vector<Entity*> graphics::entities;
	std::vector<Light*>  graphics::lights;
	std::vector<Effect*> graphics::effects;
	Vec3f graphics::light_ambient = vec3f_0;
	Vec3f graphics::bg_color = vec3f_0;


	void graphics::setBufferSize(Vec2u size) {
		graphics::size = size;
		mid = size / 2;
		bufferSize = size[X] * size[Y];
		pixels = std::vector<Vec3f>(bufferSize);
		depth = std::vector<float>(bufferSize);
		setFov(90U);
	}


	void graphics::setFov(unsigned fov) {
		camera.setFov(fov);
	}


	void graphics::setControls(bool controls[12]) {
		camera.setControls(controls);
	}

	
	void graphics::setAmbientLightColor(Vec3f color) {
		c_max(color);
		light_ambient = color;
	}

	
	void graphics::setBackgroundColor(Vec3f color) {
		c_max(color);
		bg_color = color;
	}

	
	void graphics::renderFrame(float dtime) {
		std::fill(pixels.begin(), pixels.end(), bg_color);
		std::fill(depth.begin(), depth.end(), camera.zfar);
		
		graphics::dtime = dtime;
		camera.calcTrigValues();
		
		physics::simulatePhysics();
		drawEntities();

		for (auto& e : effects) e->applyEffect(&pixels[0], &pixels[0]);
		bloom.applyEffect(&pixels[0], &pixels[0]);
		for (size_t i = 0; i < bufferSize; i++) notBloom(pixels[i]);
	}


	void graphics::drawEntities() {
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

	
	void graphics::getFrameDEC(int* frame) {
		for (size_t i = 0; i < bufferSize; ++i) {
			frame[i] = rgbToDec(pixels[i]);
		}
	}

	
	void graphics::getFrameRGB(uint8_t* frame) {
		uint8_t* f = frame;
		for (size_t i = 0; i < bufferSize; ++i) {
			*f++ = pixels[i][R];
			*f++ = pixels[i][G];
			*f++ = pixels[i][B];
		}
	}


	bool graphics::loadModel(const char* name, const char* filename, Vec3f pos, Vec3f rot) {
		if (!std::ifstream(filename).good()) return false;
		models[std::string(name)] = new Model(filename, pos, rot);
		return true;
	}


	bool graphics::loadTexture(const char* name, const char* filename, char type) {
		if (!std::ifstream(filename).good()) return false;
		if (type != 'c' && type != 'n') return false;
		textures[std::string(name)] = new Texture(filename, type);
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


	bool graphics::addEntity(GraphicsArgs ga, DynamicsArgs da, CollisionArgs ca) {
		if (!models[ga.model]) return false;
		
		PhysicsObj* physicsObj;
		switch(ca.colliderType) {
			case ColliderType::NONE: {
				physicsObj = new PhysicsObj(
					da.dynamic,
					da.pos, da.rot,
					da.posVel, da.rotVel,
					da.posAcc, da.rotAcc,
					ca.mass
				);
				break;
			}
			case ColliderType::SPHERE: {
				physicsObj = new SphereCollider(
					da.dynamic,
					da.pos, da.rot,
					da.posVel, da.rotVel,
					da.posAcc, da.rotAcc,
					ca.mass, ca.hbxrad
				);
				break;
			}
		}

		entities.push_back(new Entity(
			models[ga.model],
			ga.texture ? textures[ga.texture] : nullptr,
			ga.normalMap ? textures[ga.normalMap] : nullptr,
			mapShadingType(ga.shading),
			ga.color,
			ga.specularExponent,
			ga.specularIntensity,
			physicsObj
		));
		return true;
	}

	
	void graphics::addDirLight(
		Vec3f color,
		Vec3f dir
	) { lights.push_back(new Light_Dir(color, dir)); }


	void graphics::addPointLight(
		Vec3f color,
		Vec3f pos,
		float falloff
	) { lights.push_back(new Light_Pt(color, pos, falloff)); }


	void graphics::addSpotLight(
		Vec3f color, Vec3f pos, Vec3f dir,
		float falloff, float width, float falloffExp
	) {
		lights.push_back(new Light_Sp(color, pos, dir, falloff, width, falloffExp));
	}

	
	void graphics::addEffect(Effect* effect) { // temp
		effects.push_back(effect);
	}

	
	void graphics::removeEntity(size_t index) {
		if (index < entities.size()) {
			delete entities[index];
			entities.erase(entities.begin() + index);
		}
	}

	
	void graphics::removeLight(size_t index) {
		if (index < lights.size()) {
			delete entities[index];
			lights.erase(lights.begin() + index);
		}
	}


	void graphics::removeEffect(size_t index) {
		if (index < effects.size()) {
			delete entities[index];
			effects.erase(effects.begin() + index);
		}
	}


	Vec3f graphics::getAllLights(
		const Vec3f& pos, const Vec3f& normal,
		unsigned specularExponent, float specularIntensity
	) {
		Vec3f color = light_ambient;
		for (auto& l : lights) {
			color += l->getLight(pos, normal, specularExponent, specularIntensity);
		}
		return color;
	}


	int graphics::coordsToIndex(const Vec2& in) {
		return size[X] * in[Y] + in[X];
	}

}
