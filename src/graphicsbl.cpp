#include <thread>
#include "graphicsbl_p.h"
#include "../include/graphicsbl.h"

namespace bl {

	float  GraphicsBL_p::dtime = 0.0f;
	Camera GraphicsBL_p::camera;
	Bloom  GraphicsBL_p::bloom(50);

	Vec2   GraphicsBL_p::size = { 0 };
	Vec2   GraphicsBL_p::mid = { 0 };
	size_t GraphicsBL_p::bufferSize = 0;

	std::unique_ptr<Vec3f[]> GraphicsBL_p::pixels;
	std::unique_ptr<float[]> GraphicsBL_p::depth;

	std::vector<std::unique_ptr<Entity>> GraphicsBL_p::entities;
	std::vector<std::unique_ptr<Light>>  GraphicsBL_p::lights;
	std::vector<std::unique_ptr<Effect>> GraphicsBL_p::effects;
	Vec3f GraphicsBL_p::light_ambient = vec3f_0;
	Vec3f GraphicsBL_p::bg_color = vec3f_0;


	void GraphicsBL_p::setBufferSize(Vec2 newSize) {
		size = newSize;
		mid = size / 2;
		bufferSize = size[X] * size[Y];
		pixels.reset(new Vec3f[bufferSize]);
		depth.reset(new float[bufferSize]);
		setFov(90);
	}


	void GraphicsBL_p::setFov(unsigned fov) {
		camera.setFov(fov);
	}

	
	void GraphicsBL_p::renderFrame(float dtime) {
		std::fill(pixels.get(), pixels.get() + bufferSize, bg_color);
		std::fill(depth.get(), depth.get() + bufferSize, camera.zfar);

		GraphicsBL_p::dtime = dtime;
		camera.calcTrigValues();

		drawEntities();
		getCollisions();

		for (auto& e : effects) e->applyEffect(pixels.get(), pixels.get());
		bloom.applyEffect(pixels.get(), pixels.get());
		for (size_t i = 0; i < bufferSize; i++) notBloom(pixels[i]);
	}


	void GraphicsBL_p::drawEntities() {
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


	void GraphicsBL_p::getCollisions() {
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


	void GraphicsBL_p::getFrameDEC(int* frame) {
		for (size_t i = 0; i < bufferSize; ++i) {
			frame[i] = rgbToDec(pixels[i]);
		}
	}


	void GraphicsBL_p::getFrameRGB(uint8_t* frame) {
		uint8_t* f = frame;
		for (size_t i = 0; i < bufferSize; ++i) {
			*f++ = pixels[i][R];
			*f++ = pixels[i][G];
			*f++ = pixels[i][B];
		}
	}


	void GraphicsBL_p::addEntity(Entity* entity) {
		entities.push_back(std::unique_ptr<Entity>(entity));
	}


	void GraphicsBL_p::addLight(Light* light) {
		lights.push_back(std::unique_ptr<Light>(light));
	}


	void GraphicsBL_p::addEffect(Effect* effect) {
		effects.push_back(std::unique_ptr<Effect>(effect));
	}


	void GraphicsBL_p::setAmbientLightColor(Vec3f color) {
		c_max(color);
		light_ambient = color;
	}

	void GraphicsBL_p::setBackgroundColor(Vec3f color) {
		c_max(color);
		bg_color = color;
	}


	void GraphicsBL_p::removeEntity(size_t index) {
		if (entities.size()) {
			index %= entities.size();
			entities.erase(entities.begin() + index);
		}
	}


	void GraphicsBL_p::removeLight(size_t index) {
		if (lights.size()) {
			index %= lights.size();
			lights.erase(lights.begin() + index);
		}
	}


	void GraphicsBL_p::removeEffect(size_t index) {
		if (effects.size()) {
			index %= effects.size();
			effects.erase(effects.begin() + index);
		}
	}


	int GraphicsBL_p::coordsToIndex(const Vec2& in) {
		return size[X] * in[Y] + in[X];
	}


	///////////////////////////////////////////////////////////////////////////
	////////////////////          PUBLIC    CLASS          ////////////////////
	///////////////////////////////////////////////////////////////////////////

	void GraphicsBL::setBufferSize(Vec2 size) {
		GraphicsBL_p::setBufferSize(size);
	}


	void GraphicsBL::setFov(unsigned fov) {
		GraphicsBL_p::setFov(fov);
	}


	void GraphicsBL::setControls(bool controls[12]) {
		GraphicsBL_p::camera.setControls(controls);
	}

	
	void GraphicsBL::renderFrame(float dtime) {
		GraphicsBL_p::renderFrame(dtime);
	}

	
	void GraphicsBL::getFrameDEC(int* frame) {
		GraphicsBL_p::getFrameDEC(frame);
	}

	
	void GraphicsBL::getFrameRGB(uint8_t* frame) {
		GraphicsBL_p::getFrameRGB(frame);
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

	
	void GraphicsBL::addEntity(
		const char* filename, ShadingType shading, Vec3f color, Vec3f pos,
		bool collision, float hbxrad, float mass,
		bool movement, Vec3f velocity, Vec3f acceleration
	) {
		GraphicsBL_p::addEntity(new Entity(
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
	}

	
	void GraphicsBL::addDirLight(
		Vec3f color,
		Vec3f dir
	) { GraphicsBL_p::addLight(new Light_Dir(color, dir)); }


	void GraphicsBL::addPointLight(
		Vec3f color,
		Vec3f pos,
		float falloff
	) { GraphicsBL_p::addLight(new Light_Pt(color, pos, falloff)); }


	void GraphicsBL::addSpotLight(
		Vec3f color, Vec3f pos, Vec3f dir,
		float falloff, float width, float falloffExp
	) {
		GraphicsBL_p::addLight(new Light_Sp(color, pos, dir, falloff, width, falloffExp));
	}

	
	void GraphicsBL::addEffect(Effect* effect) { // temp
		GraphicsBL_p::addEffect(effect);
	}

	
	void GraphicsBL::setAmbientLightColor(Vec3f color) {
		GraphicsBL_p::setAmbientLightColor(color);
	}

	
	void GraphicsBL::setBackgroundColor(Vec3f color) {
		GraphicsBL_p::setBackgroundColor(color);
	}

	
	void GraphicsBL::removeEntity(size_t index) {
		GraphicsBL_p::removeEntity(index);
	}

	
	void GraphicsBL::removeLight(size_t index) {
		GraphicsBL_p::removeLight(index);
	}


	void GraphicsBL::removeEffect(size_t index) {
		GraphicsBL_p::removeEffect(index);
	}

}
