#include "pixelshader.h"
#include "graphics.h"

namespace ramiel {

    template<class Vertex>
    static inline Vec3f getNormal(Vertex v[3]) {
		return getNormalized(crossProduct(
            v[1].worldPos - v[0].worldPos,
            v[2].worldPos - v[0].worldPos
        ));
    }


    void PS_PerTri::init(Vertex_PerTri v[3]) {
        Vec3f pos = (v[0].worldPos + v[1].worldPos + v[2].worldPos) / 3.0f;
		Vec3f normal = getNormal(v);
		color = surfaceColor * graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
    }


    Vec3f PS_PerTri::draw(const Vertex_PerTri& v) {
        return color;
    }


    void PS_PerTri_Textured::init(Vertex_PerTri_Textured v[3]) {
        Vec3f pos = (v[0].worldPos + v[1].worldPos + v[2].worldPos) / 3.0f;
		Vec3f normal = getNormal(v);
		light = graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
        for (size_t i = 0; i < 3; ++i) v[i].texturePos *= v[i].zinv;
    }


    Vec3f PS_PerTri_Textured::draw(const Vertex_PerTri_Textured& v) {
        return light * texture->get(v.texturePos / v.zinv);
    }


    void PS_PerVertex::init(Vertex_PerVertex v[3]) {
        return;
    }


    Vec3f PS_PerVertex::draw(const Vertex_PerVertex& v) {
        return v.color;
    }


    void PS_PerVertex_Textured::init(Vertex_PerVertex_Textured v[3]) {
        for (size_t i = 0; i < 3; ++i) v[i].texturePos *= v[i].zinv;
    }


    Vec3f PS_PerVertex_Textured::draw(const Vertex_PerVertex_Textured& v) {
        return v.color * texture->get(v.texturePos / v.zinv);
    }


    void PS_PerPixel::init(Vertex_PerPixel v[3]) {
		normal = getNormal(v);
		for (int i = 0; i < 3; ++i) v[i].worldPos *= v[i].zinv;
    }


    Vec3f PS_PerPixel::draw(const Vertex_PerPixel& v) {
        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
		graphics::pixels[i] = color * surfaceColor;
    }


    void PS_PerPixel_Textured::init(Vertex_PerPixel_Textured v[3]) {
        normal = getNormal(v);
		for (int i = 0; i < 3; ++i) {
            v[i].worldPos *= v[i].zinv;
            v[i].texturePos *= v[i].zinv;
        }
    }


    Vec3f PS_PerPixel_Textured::draw(const Vertex_PerPixel_Textured& v) {
        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
		graphics::pixels[i] = color * texture->get(v.texturePos / v.zinv);
    }


    void PS_PerPixel_NormalMapped::init(Vertex_PerPixel_Textured v[3]) {
        const Vec3f dpos1 = v[1].worldPos - v[0].worldPos;
        const Vec3f dpos2 = v[2].worldPos - v[0].worldPos;
        const Vec2f dtex1 = v[1].texturePos - v[0].texturePos;
        const Vec2f dtex2 = v[2].texturePos - v[0].texturePos;

        const float f = 1.0f / (dtex1[X] * dtex2[Y] + dtex2[X] * dtex1[Y]);
        normal    = crossProduct(dpos1, dpos2);
        tangent   = (dpos1 * dtex2[Y] - dpos2 * dtex1[Y]) * f;
        bitangent = (dpos2 * dtex1[X] - dpos1 * dtex2[X]) * f;

        normal    = getNormalized(normal);
        tangent   = getNormalized(tangent);
        bitangent = getNormalized(bitangent);

		for (int i = 0; i < 3; ++i) {
            v[i].worldPos *= v[i].zinv;
            v[i].texturePos *= v[i].zinv;
        }
    }


    Vec3f PS_PerPixel_NormalMapped::draw(const Vertex_PerPixel_Textured& v) {
        Vec3f normal_in = normalmap->get(v.texturePos / v.zinv);
		Vec3f normal_out = tangent * normal_in[X] + bitangent * normal_in[Y] + normal * normal_in[Z];

        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal_out, specularExponent, specularIntensity);
		graphics::pixels[i] = color * surfaceColor;
    }


    void PS_PerPixel_Textured_NormalMapped::init(Vertex_PerPixel_Textured v[3]) {
        const Vec3f dpos1 = v[1].worldPos - v[0].worldPos;
        const Vec3f dpos2 = v[2].worldPos - v[0].worldPos;
        const Vec2f dtex1 = v[1].texturePos - v[0].texturePos;
        const Vec2f dtex2 = v[2].texturePos - v[0].texturePos;

        const float f = 1.0f / (dtex1[X] * dtex2[Y] + dtex2[X] * dtex1[Y]);
        normal    = crossProduct(dpos1, dpos2);
        tangent   = (dpos1 * dtex2[Y] - dpos2 * dtex1[Y]) * f;
        bitangent = (dpos2 * dtex1[X] - dpos1 * dtex2[X]) * f;

        normal    = getNormalized(normal);
        tangent   = getNormalized(tangent);
        bitangent = getNormalized(bitangent);
        
		for (int i = 0; i < 3; ++i) {
            v[i].worldPos *= v[i].zinv;
            v[i].texturePos *= v[i].zinv;
        }
    }


    Vec3f PS_PerPixel_Textured_NormalMapped::draw(const Vertex_PerPixel_Textured& v) {
        Vec3f normal_in = normalmap->get(v.texturePos / v.zinv);
		Vec3f normal_out = tangent * normal_in[X] + bitangent * normal_in[Y] + normal * normal_in[Z];

        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal_out, specularExponent, specularIntensity);
		graphics::pixels[i] = color * texture->get(v.texturePos / v.zinv);
    }


    void PS_PerPixel_Smooth::init(Vertex_PerPixel_Smooth v[3]) {
        for (int i = 0; i < 3; ++i) v[i].worldPos *= v[i].zinv;
    }


    Vec3f PS_PerPixel_Smooth::draw(const Vertex_PerPixel_Smooth& v) {
        Vec3f normal = getNormalized(v.normal);

        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
		graphics::pixels[i] = color * surfaceColor;
    }


    void PS_PerPixel_Smooth_Textured::init(Vertex_PerPixel_Smooth_Textured v[3]) {
        for (int i = 0; i < 3; ++i) {
            v[i].worldPos *= v[i].zinv;
            v[i].texturePos *= v[i].zinv;
        }
    }


    Vec3f PS_PerPixel_Smooth_Textured::draw(const Vertex_PerPixel_Smooth_Textured& v) {
        Vec3f normal = getNormalized(v.normal);

        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
		graphics::pixels[i] = color * texture->get(v.texturePos / v.zinv);
    }

}
