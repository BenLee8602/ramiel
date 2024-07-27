#pragma once

#include <ramiel/data.h>
#include "light.h"

namespace ramiel {

    template<class Vertex>
    inline Vec3f getNormal(Vertex v[3]) {
        return normalize(cross(
            v[1].worldPos - v[0].worldPos,
            v[2].worldPos - v[0].worldPos
        ));
    }


    class PS_PerTri {
        LightingListSpecular lightingList;
        Vec3f surfaceColor;
        Vec3f color;
    public:
        PS_PerTri(
            LightingListSpecular lightingList,
            Vec3f surfaceColor
        ) :
            lightingList(lightingList),
            surfaceColor(surfaceColor / 255.0f),
            color(Vec3f())
        {}

        template<class Vertex_In>
        void init(Vertex_In v[3]) {
            Vec3f pos = (v[0].worldPos + v[1].worldPos + v[2].worldPos) / 3.0f;
		    Vec3f normal = getNormal(v);
		    color = surfaceColor * lightingList.getAllLight(pos, normal);
        }

        template<class Vertex_In>
        Vec3f draw(const Vertex_In& v) {
            return color;
        }
    };


    class PS_PerTri_Textured {
        LightingListSpecular lightingList;
        const Texture* texture;
        Vec3f light;
    public:
        PS_PerTri_Textured(
            LightingListSpecular lightingList,
            const Texture* texture
        ) :
            lightingList(lightingList),
            texture(texture),
            light(Vec3f())
        {}

        template<class Vertex_In>
        void init(Vertex_In v[3]) {
            Vec3f pos = (v[0].worldPos + v[1].worldPos + v[2].worldPos) / 3.0f;
            Vec3f normal = getNormal(v);
            light = lightingList.getAllLight(pos, normal);
        }

        template<class Vertex_In>
        Vec3f draw(const Vertex_In& v) {
            return light * texture->get(v.texturePos);
        }
    };


    class PS_PerVertex {
    public:
        template<class Vertex_In>
        void init(Vertex_In v[3]) {}

        template<class Vertex_In>
        Vec3f draw(const Vertex_In& v) {
            return v.color;
        }
    };


    class PS_PerVertex_Textured {
        const Texture* texture;
    public:
        PS_PerVertex_Textured(const Texture* texture) : texture(texture) {}

        template<class Vertex_In>
        void init(Vertex_In v[3]) {}

        template<class Vertex_In>
        Vec3f draw(const Vertex_In& v) {
            return v.light * texture->get(v.texturePos);
        }
    };


    class PS_PerPixel {
        LightingListSpecular lightingList;
        Vec3f surfaceColor;
        Vec3f normal;
    public:
        PS_PerPixel(
            LightingListSpecular lightingList,
            Vec3f surfaceColor
        ) :
            lightingList(lightingList),
            surfaceColor(surfaceColor / 255.0f),
            normal(Vec3f())
        {}

        template<class Vertex_In>
        void init(Vertex_In v[3]) {
            normal = getNormal(v);
        }

        template<class Vertex_In>
        Vec3f draw(const Vertex_In& v) {
            Vec3f color = lightingList.getAllLight(v.worldPos, normal);
            return color * surfaceColor;
        }
    };


    class PS_PerPixel_Textured {
        LightingListSpecular lightingList;
        const Texture* texture;
        Vec3f normal;
    public:
        PS_PerPixel_Textured(
            LightingListSpecular lightingList,
            const Texture* texture
        ) :
            lightingList(lightingList),
            texture(texture),
            normal(Vec3f())
        {}

        template<class Vertex_In>
        void init(Vertex_In v[3]) {
            normal = getNormal(v);
        }

        template<class Vertex_In>
        Vec3f draw(const Vertex_In& v) {
            Vec3f color = lightingList.getAllLight(v.worldPos, normal);
            return color * texture->get(v.texturePos);
        }
    };


    class PS_PerPixel_NormalMapped {
        LightingListSpecular lightingList;
        const Texture* normalMap;
        Vec3f surfaceColor;
        Vec3f normal;
        Vec3f tangent;
        Vec3f bitangent;
    public:
        PS_PerPixel_NormalMapped(
            LightingListSpecular lightingList,
            const Texture* normalMap,
            Vec3f surfaceColor
        ) :
            lightingList(lightingList),
            normalMap(normalMap),
            surfaceColor(surfaceColor / 255.0f),
            normal(Vec3f()),
            tangent(Vec3f()),
            bitangent(Vec3f())
        {}

        template<class Vertex_In>
        void init(Vertex_In v[3]) {
            const Vec3f dpos1 = v[1].worldPos - v[0].worldPos;
            const Vec3f dpos2 = v[2].worldPos - v[0].worldPos;
            const Vec2f dtex1 = v[1].texturePos - v[0].texturePos;
            const Vec2f dtex2 = v[2].texturePos - v[0].texturePos;

            const float f = 1.0f / (dtex1[X] * dtex2[Y] + dtex2[X] * dtex1[Y]);
            normal    = cross(dpos1, dpos2);
            tangent   = (dpos1 * dtex2[Y] - dpos2 * dtex1[Y]) * f;
            bitangent = (dpos2 * dtex1[X] - dpos1 * dtex2[X]) * f;

            normal    = normalize(normal);
            tangent   = normalize(tangent);
            bitangent = normalize(bitangent);
        }

        template<class Vertex_In>
        Vec3f draw(const Vertex_In& v) {
            Vec3f normal_in = normalMap->get(v.texturePos);
            Vec3f normal_out = tangent * normal_in[X] + bitangent * normal_in[Y] + normal * normal_in[Z];
            Vec3f pos = v.worldPos;
            Vec3f color = lightingList.getAllLight(pos, normal_out);
            return color * surfaceColor;
        }
    };


    class PS_PerPixel_Textured_NormalMapped {
        LightingListSpecular lightingList;
        const Texture* texture;
        const Texture* normalMap;
        Vec3f normal;
        Vec3f tangent;
        Vec3f bitangent;
    public:
        PS_PerPixel_Textured_NormalMapped(
            LightingListSpecular lightingList,
            const Texture* texture,
            const Texture* normalMap
        ) :
            lightingList(lightingList),
            texture(texture),
            normalMap(normalMap),
            normal(Vec3f()),
            tangent(Vec3f()),
            bitangent(Vec3f())
        {}

        template<class Vertex_In>
        void init(Vertex_In v[3]) {
            const Vec3f dpos1 = v[1].worldPos - v[0].worldPos;
            const Vec3f dpos2 = v[2].worldPos - v[0].worldPos;
            const Vec2f dtex1 = v[1].texturePos - v[0].texturePos;
            const Vec2f dtex2 = v[2].texturePos - v[0].texturePos;

            const float f = 1.0f / (dtex1[X] * dtex2[Y] + dtex2[X] * dtex1[Y]);
            normal    = cross(dpos1, dpos2);
            tangent   = (dpos1 * dtex2[Y] - dpos2 * dtex1[Y]) * f;
            bitangent = (dpos2 * dtex1[X] - dpos1 * dtex2[X]) * f;

            normal    = normalize(normal);
            tangent   = normalize(tangent);
            bitangent = normalize(bitangent);
        }

        template<class Vertex_In>
        Vec3f draw(const Vertex_In& v) {
            Vec3f normal_in = normalMap->get(v.texturePos);
            Vec3f normal_out = tangent * normal_in[X] + bitangent * normal_in[Y] + normal * normal_in[Z];
            Vec3f color = lightingList.getAllLight(v.worldPos, normal_out);
            return color * texture->get(v.texturePos);
        }
    };


    class PS_PerPixel_Smooth {
        LightingListSpecular lightingList;
        Vec3f surfaceColor;
    public:
        PS_PerPixel_Smooth(
            LightingListSpecular lightingList,
            Vec3f surfaceColor
        ) :
            lightingList(lightingList),
            surfaceColor(surfaceColor / 255.0f)
        {}

        template<class Vertex_In>
        void init(Vertex_In v[3]) {}

        template<class Vertex_In>
        Vec3f draw(const Vertex_In& v) {
            Vec3f normal = normalize(v.normal);
            Vec3f color = lightingList.getAllLight(v.worldPos, normal);
            return color * surfaceColor;
        }
    };


    class PS_PerPixel_Smooth_Textured {
        LightingListSpecular lightingList;
        const Texture* texture;
    public:
        PS_PerPixel_Smooth_Textured(
            LightingListSpecular lightingList,
            const Texture* texture
        ) :
            lightingList(lightingList),
            texture(texture)
        {}

        template<class Vertex_In>
        void init(Vertex_In v[3]) {}

        template<class Vertex_In>
        Vec3f draw(const Vertex_In& v) {
            Vec3f normal = normalize(v.normal);
            Vec3f color = lightingList.getAllLight(v.worldPos, normal);
            return color * texture->get(v.texturePos);
        }
    };

}
