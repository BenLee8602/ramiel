#pragma once

#include <unordered_map>
#include <string>
#include <variant>

#include "ramiel.h"

namespace ramiel {

    typedef std::unordered_map<std::string, std::variant<
		bool, float, unsigned, std::string,
		Vec3f, ShadingType, ColliderType, LightType
	>> Args;

    namespace graphics {

        bool addEntity(Args args);
        bool addLight(Args args);

    }

}


#ifdef RAMIEL_ARGS_IMPLEMENTATION

#include <iostream>

namespace ramiel {

    template<typename T>
	void getArg(Args& args, const std::string& key, T& out) {
		auto i = args.find(key);
		if (i != args.end()) {
			out = std::get<T>(i->second);
		}
	}

    bool graphics::addEntity(Args args) {
        // mandatory
		if (args.find("mesh") == args.end()) return false;

		// defaults
		std::string mesh;
		ShadingType shading = ShadingType::FLAT;
		Vec3f color = vec3f_255;
		std::string texture;
		std::string normalMap;
		unsigned specularExponent = 0U;
		float specularIntensity = 0.0f;
		Vec3f pos    = vec3f_0;
		Vec3f rot    = vec3f_0;
		bool dynamic = false;
		Vec3f posVel = vec3f_0;
		Vec3f rotVel = vec3f_0;
		Vec3f posAcc = vec3f_0;
		Vec3f rotAcc = vec3f_0;
		ColliderType colliderType = ColliderType::NONE;
		float mass = 1.0f;
		float hbxrad = 0.5f;
		
		// get args
		try {
			getArg(args, "mesh", mesh);
			getArg(args, "shading", shading);
			getArg(args, "color", color);
			getArg(args, "texture", texture);
			getArg(args, "normalMap", normalMap);
			getArg(args, "specularExponent", specularExponent);
			getArg(args, "specularIntensity", specularIntensity);
			getArg(args, "pos", pos);
			getArg(args, "rot", rot);
			getArg(args, "dynamic", dynamic);
			getArg(args, "posVel", posVel);
			getArg(args, "rotVel", rotVel);
			getArg(args, "posAcc", posAcc);
			getArg(args, "rotAcc", rotAcc);
			getArg(args, "colliderType", colliderType);
			getArg(args, "mass", mass);
			if (colliderType == ColliderType::SPHERE) {
				getArg(args, "hbxrad", hbxrad);
			}
		} catch (std::bad_variant_access e) {
			std::cerr << "bad argument types - ramiel::graphics::addEntity(Args)\n";
			return false;
		}

		return addEntity(
			mesh, shading,
			color, texture, normalMap,
			specularExponent, specularIntensity,
			pos, rot,
			dynamic,
			posVel, rotVel,
			posAcc, rotAcc,
			colliderType,
			mass, hbxrad
		);
	}


    bool graphics::addLight(Args args) {
        // mandatory
        if (args.find("lightType") == args.end()) return false;

        // defaults
        LightType lightType;
        Vec3f color = vec3f_255;
        Vec3f dir = vec3f_0;
        Vec3f pos = vec3f_0;
        float falloff = -1.0f;
        float width = 30.0f;
        float falloffExponent = 50.0f;

        // get args
        try {
            getArg(args, "lightType", lightType);
            getArg(args, "color", color);
            getArg(args, "dir", dir);
            getArg(args, "pos", pos);
            getArg(args, "falloff", falloff);
            getArg(args, "width", width);
            getArg(args, "falloffExponent", falloffExponent);
        } catch (std::bad_variant_access e) {
            std::cerr << "bad argument types - ramiel::graphics::addEntity(Args)\n";
            return false;
        }

        switch (lightType) {
            case LightType::DIR:   addDirLight(color, dir); break;
            case LightType::POINT: addPointLight(color, pos, falloff); break;
            case LightType::SPOT:  addSpotLight(color, pos, dir, falloff, width, falloffExponent);
        }

        return true;
    }

}

#endif
