#include "vertexshader.h"


namespace ramiel {

    inline void rotate(Vec3f& v, const Vec3f& sin, const Vec3f& cos) {
        Vec3f out = v;

        // z rot
        out[X] = v[X] * cos[Z] + v[Y] * -sin[Z];
        out[Y] = v[X] * sin[Z] + v[Y] * cos[Z];
        v = out;

        // y rot
        out[X] = v[X] * cos[Y] + v[Z] * sin[Y];
        out[Z] = v[X] * -sin[Y] + v[Z] * cos[Y];
        v = out;

        // x rot
        out[Y] = v[Y] * cos[X] + v[Z] * -sin[X];
        out[Z] = v[Y] * sin[X] + v[Z] * cos[X];
        v = out;
    }


    template<class Vertex>
    void transformVPos(const std::vector<Vec3f>& v_pos, std::vector<Vertex>& out, float scale, Vec3f pos, Vec3f rot) {
        if (scale != 1.0f) for (auto& v : v_pos) v *= scale;
        if (rot) {
            Vec3f sin = { std::sin(rot[X]), std::sin(rot[Y]), std::sin(rot[Z]) };
            Vec3f cos = { std::cos(rot[X]), std::cos(rot[Y]), std::cos(rot[Z]) };
            for (auto& v : v_pos) rotate(v, sin, cos);
        }
        if (pos) for (auto& v : v_pos) v += pos;
    }


    template<class Vertex>
    void transformVNormal(const std::vector<Vec3f>& v_normal, std::vector<Vertex>& out, Vec3f rot) {
        if (!rot) return;
        Vec3f sin = { std::sin(rot[X]), std::sin(rot[Y]), std::sin(rot[Z]) };
        Vec3f cos = { std::cos(rot[X]), std::cos(rot[Y]), std::cos(rot[Z]) };
        for (auto& v : v_normal) rotate(v, sin, cos);
    }


    void VS_PerTri(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();

        std::vector<Vertex_PerTri> vertices(v_pos.size());
        transformVPos(v_pos, vertices, scale, physicsObj->getPos(), physicsObj->getRot());

        const std::vector<Vec3u>& tri = mesh->getTri();
    }


    void VS_PerTri_Textured(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec2f>& v_txt = mesh->getVTxt();

        std::vector<Vertex_PerTri_Textured> vertices(v_pos.size());
        transformVPos(v_pos, vertices, scale, physicsObj->getPos(), physicsObj->getRot());

        const std::vector<Vec3u>& tri = mesh->getTri();
        const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();
    }

    
    void VS_PerVertex(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec3f>& v_normal = mesh->getVNormal();

        std::vector<Vertex_PerVertex> vertices(v_pos.size());
        transformVPos(v_pos, vertices, scale, physicsObj->getPos(), physicsObj->getRot());
        transformVNormal(v_normal, vertices, physicsObj->getRot());

        const std::vector<Vec3u>& tri = mesh->getTri();
    }

    
    void VS_PerVertex_Textured(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec3f>& v_normal = mesh->getVNormal();
        const std::vector<Vec2f>& v_txt = mesh->getVTxt();

        std::vector<Vertex_PerVertex_Textured> vertices(v_pos.size());
        transformVPos(v_pos, vertices, scale, physicsObj->getPos(), physicsObj->getRot());
        transformVNormal(v_normal, vertices, physicsObj->getRot());

        const std::vector<Vec3u>& tri = mesh->getTri();
        const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();
    }

    
    void VS_PerPixel(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();

        std::vector<Vertex_PerPixel> vertices(v_pos.size());
        transformVPos(v_pos, vertices, scale, physicsObj->getPos(), physicsObj->getRot());

        const std::vector<Vec3u>& tri = mesh->getTri();
    }

    
    void VS_PerPixel_Textured(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec2f>& v_txt = mesh->getVTxt();

        std::vector<Vertex_PerPixel_Textured> vertices(v_pos.size());
        transformVPos(v_pos, vertices, scale, physicsObj->getPos(), physicsObj->getRot());

        const std::vector<Vec3u>& tri = mesh->getTri();
        const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();
    }

    
    void VS_PerPixel_Smooth(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec3f>& v_normal = mesh->getVNormal();

        std::vector<Vertex_PerPixel_Smooth> vertices(v_pos.size());
        transformVPos(v_pos, vertices, scale, physicsObj->getPos(), physicsObj->getRot());
        transformVNormal(v_normal, vertices, physicsObj->getRot());

        const std::vector<Vec3u>& tri = mesh->getTri();
    }

    
    void VS_PerPixel_Smooth_Textured(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec3f>& v_normal = mesh->getVNormal();
        const std::vector<Vec2f>& v_txt = mesh->getVTxt();

        std::vector<Vertex_PerPixel_Smooth_Textured> vertices(v_pos.size());
        transformVPos(v_pos, vertices, scale, physicsObj->getPos(), physicsObj->getRot());
        transformVNormal(v_normal, vertices, physicsObj->getRot());

        const std::vector<Vec3u>& tri = mesh->getTri();
        const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();
    }

}
