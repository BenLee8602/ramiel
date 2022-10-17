#include "pixelshader.h"
#include "graphics.h"

namespace ramiel {

    bool PS_PerTri::init(Vertex_PerTri v[3]) {
        
    }


    Vec3f PS_PerTri::draw(const Vertex_PerTri& v) {
        return color;
    }


    bool PS_PerTri_Textured::init(Vertex_PerTri_Textured v[3]) {

    }


    Vec3f PS_PerTri_Textured::draw(const Vertex_PerTri_Textured& v) {
        return color * texture->get(v.uv / v.zinv);
    }


    bool PS_PerVertex::init(Vertex_PerVertex v[3]) {
        
    }


    Vec3f PS_PerVertex::draw(const Vertex_PerVertex& v) {
        return v.color;
    }


    bool PS_PerVertex_Textured::init(Vertex_PerVertex_Textured v[3]) {
        
    }


    Vec3f PS_PerVertex_Textured::draw(const Vertex_PerVertex_Textured& v) {
        return v.color * texture->get(v.uv / v.zinv);
    }


    bool PS_PerPixel::init(Vertex_PerPixel v[3]) {

    }


    Vec3f PS_PerPixel::draw(const Vertex_PerPixel& v) {
        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
		graphics::pixels[i] = color * surfaceColor;
    }


    bool PS_PerPixel_Textured::init(Vertex_PerPixel_Textured v[3]) {

    }


    Vec3f PS_PerPixel_Textured::draw(const Vertex_PerPixel_Textured& v) {
        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
		graphics::pixels[i] = color * texture->get(v.uv / v.zinv);
    }


    bool PS_PerPixel_NormalMapped::init(Vertex_PerPixel_Textured v[3]) {

    }


    Vec3f PS_PerPixel_NormalMapped::draw(const Vertex_PerPixel_Textured& v) {
        Vec3f normal_in = normalmap->get(v.uv / v.zinv);
		Vec3f normal_out = tangent * normal_in[X] + bitangent * normal_in[Y] + normal * normal_in[Z];

        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal_out, specularExponent, specularIntensity);
		graphics::pixels[i] = color * surfaceColor;
    }


    bool PS_PerPixel_Textured_NormalMapped::init(Vertex_PerPixel_Textured v[3]) {

    }


    Vec3f PS_PerPixel_Textured_NormalMapped::draw(const Vertex_PerPixel_Textured& v) {
        Vec3f normal_in = normalmap->get(v.uv / v.zinv);
		Vec3f normal_out = tangent * normal_in[X] + bitangent * normal_in[Y] + normal * normal_in[Z];

        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal_out, specularExponent, specularIntensity);
		graphics::pixels[i] = color * texture->get(v.uv / v.zinv);
    }


    bool PS_PerPixel_Smooth::init(Vertex_PerPixel_Smooth v[3]) {

    }


    Vec3f PS_PerPixel_Smooth::draw(const Vertex_PerPixel_Smooth& v) {
        Vec3f normal = getNormalized(v.normal);

        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
		graphics::pixels[i] = color * surfaceColor;
    }


    bool PS_PerPixel_Smooth_Textured::init(Vertex_PerPixel_Smooth_Textured v[3]) {

    }


    Vec3f PS_PerPixel_Smooth_Textured::draw(const Vertex_PerPixel_Smooth_Textured& v) {
        Vec3f normal = getNormalized(v.normal);

        size_t i = graphics::coordsToIndex(v.screenPos);
        graphics::depth[i] = v.cameraPos;
		Vec3f pos = v.worldPos / v.zinv;
		Vec3f color = graphics::getAllLights(pos, normal, specularExponent, specularIntensity);
		graphics::pixels[i] = color * texture->get(v.uv / v.zinv);
    }

}
