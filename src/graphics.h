#pragma once

#include <unordered_map>
#include "entitybase.h"
#include "meshbase.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "effects.h"


namespace ramiel::graphics {
	
	extern Camera camera;

	extern Vec2u  size;
	extern Vec2u  mid;
	extern size_t bufferSize;

	extern std::vector<Vec3f> pixels;
	extern std::vector<float> depth;

	extern std::unordered_map<std::string, MeshBase*> meshes;
	extern std::unordered_map<std::string, Texture*>  textures;

	extern std::vector<EntityBase*> entities;
	extern std::vector<Light*> lights;
	extern std::vector<Effect> effects;
	extern Vec3f light_ambient;
	extern Vec3f bg_color;

	Vec3f getAllLights(
		const Vec3f& pos, const Vec3f& normal,
		unsigned specularExponent, float specularIntensity
	);

	size_t coordsToIndex(const Vec2f& in);

}
