#include "vertexshader.h"
#include "graphics.h"


namespace ramiel {

    template<typename Vertex, class PS>
    static void drawAllTriangles(const std::vector<Vertex>& vertices, PixelShader* ps_ptr) {
        PS pixelShader = ps_ptr->get();
        
    }


    void VS_PerTri(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();

        const Vec3f& pos = physicsObj->getPos();
        const Rotation& rot = physicsObj->getRot();

        std::vector<Vertex_PerTri> vertices(v_pos.size());
        for (size_t i = 0; i < v_pos.size(); ++i) {
            vertices[i].worldPos = rot.rotate(v_pos[i]) + pos;
            vertices[i].cameraPos = graphics::camera.getCameraCoord(vertices[i].worldPos);
            vertices[i].screenPos = graphics::camera.getScreenCoord(vertices[i].cameraPos);
        }

        const std::vector<Vec3u>& tri = mesh->getTri();
    }


    void VS_PerTri_Textured(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec2f>& v_txt = mesh->getVTxt();

        const Vec3f& pos = physicsObj->getPos();
        const Rotation& rot = physicsObj->getRot();

        std::vector<Vertex_PerTri_Textured> vertices(v_pos.size());
        for (size_t i = 0; i < v_pos.size(); ++i) {
            vertices[i].worldPos = rot.rotate(v_pos[i]) + pos;
            vertices[i].cameraPos = graphics::camera.getCameraCoord(vertices[i].worldPos);
            vertices[i].screenPos = graphics::camera.getScreenCoord(vertices[i].cameraPos);
            vertices[i].zinv = 1.0f / vertices[i].cameraPos[Z];
            vertices[i].texturePos = v_txt[i];
        }

        const std::vector<Vec3u>& tri = mesh->getTri();
        const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();
    }

    
    void VS_PerVertex(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec3f>& v_normal = mesh->getVNormal();

        const Vec3f& pos = physicsObj->getPos();
        const Rotation& rot = physicsObj->getRot();

        std::vector<Vertex_PerVertex> vertices(v_pos.size());
        for (size_t i = 0; i < v_pos.size(); ++i) {
            Vec3f worldPos = rot.rotate(v_pos[i]) + pos;
            Vec3f normal = rot.rotate(v_normal[i]);
            vertices[i].cameraPos = graphics::camera.getCameraCoord(worldPos);
            vertices[i].screenPos = graphics::camera.getScreenCoord(vertices[i].cameraPos);
            vertices[i].color = pixelShader.getAllLights(worldPos, normal);
        }

        const std::vector<Vec3u>& tri = mesh->getTri();
    }

    
    void VS_PerVertex_Textured(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec3f>& v_normal = mesh->getVNormal();
        const std::vector<Vec2f>& v_txt = mesh->getVTxt();

        const Vec3f& pos = physicsObj->getPos();
        const Rotation& rot = physicsObj->getRot();

        std::vector<Vertex_PerVertex_Textured> vertices(v_pos.size());
        for (size_t i = 0; i < v_pos.size(); ++i) {
            Vec3f worldPos = rot.rotate(v_pos[i]) + pos;
            Vec3f normal = rot.rotate(v_normal[i]);
            vertices[i].cameraPos = graphics::camera.getCameraCoord(worldPos);
            vertices[i].screenPos = graphics::camera.getScreenCoord(vertices[i].cameraPos);
            vertices[i].color = pixelShader.getAllLights(worldPos, normal);
            vertices[i].zinv = 1.0f / vertices[i].cameraPos[Z];
            vertices[i].texturePos = v_txt[i];
        }

        const std::vector<Vec3u>& tri = mesh->getTri();
        const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();
    }

    
    void VS_PerPixel(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();

        const Vec3f& pos = physicsObj->getPos();
        const Rotation& rot = physicsObj->getRot();

        std::vector<Vertex_PerPixel> vertices(v_pos.size());
        for (size_t i = 0; i < v_pos.size(); ++i) {
            vertices[i].worldPos = rot.rotate(v_pos[i]) + pos;
            vertices[i].cameraPos = graphics::camera.getCameraCoord(vertices[i].worldPos);
            vertices[i].screenPos = graphics::camera.getScreenCoord(vertices[i].cameraPos);
            vertices[i].zinv = 1.0f / vertices[i].cameraPos[Z];
        }

        const std::vector<Vec3u>& tri = mesh->getTri();
    }

    
    void VS_PerPixel_Textured(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec2f>& v_txt = mesh->getVTxt();

        const Vec3f& pos = physicsObj->getPos();
        const Rotation& rot = physicsObj->getRot();

        std::vector<Vertex_PerPixel_Textured> vertices(v_pos.size());
        for (size_t i = 0; i < v_pos.size(); ++i) {
            vertices[i].worldPos = rot.rotate(v_pos[i]) + pos;
            vertices[i].cameraPos = graphics::camera.getCameraCoord(vertices[i].worldPos);
            vertices[i].screenPos = graphics::camera.getScreenCoord(vertices[i].cameraPos);
            vertices[i].zinv = 1.0f / vertices[i].cameraPos[Z];
            vertices[i].texturePos = v_txt[i];
        }

        const std::vector<Vec3u>& tri = mesh->getTri();
        const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();
    }

    
    void VS_PerPixel_Smooth(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec3f>& v_normal = mesh->getVNormal();

        const Vec3f& pos = physicsObj->getPos();
        const Rotation& rot = physicsObj->getRot();

        std::vector<Vertex_PerPixel_Smooth> vertices(v_pos.size());
        for (size_t i = 0; i < v_pos.size(); ++i) {
            vertices[i].worldPos = rot.rotate(v_pos[i]) + pos;
            vertices[i].cameraPos = graphics::camera.getCameraCoord(vertices[i].worldPos);
            vertices[i].screenPos = graphics::camera.getScreenCoord(vertices[i].cameraPos);
            vertices[i].normal = rot.rotate(v_normal[i]);
            vertices[i].zinv = 1.0f / vertices[i].cameraPos[Z];
        }

        const std::vector<Vec3u>& tri = mesh->getTri();
    }

    
    void VS_PerPixel_Smooth_Textured(VERTEX_SHADER_ARGS) {
		const std::vector<Vec3f>& v_pos = mesh->getVPos();
        const std::vector<Vec3f>& v_normal = mesh->getVNormal();
        const std::vector<Vec2f>& v_txt = mesh->getVTxt();

        const Vec3f& pos = physicsObj->getPos();
        const Rotation& rot = physicsObj->getRot();

        std::vector<Vertex_PerPixel_Smooth_Textured> vertices(v_pos.size());
        for (size_t i = 0; i < v_pos.size(); ++i) {
            vertices[i].worldPos = rot.rotate(v_pos[i]) + pos;
            vertices[i].cameraPos = graphics::camera.getCameraCoord(vertices[i].worldPos);
            vertices[i].screenPos = graphics::camera.getScreenCoord(vertices[i].cameraPos);
            vertices[i].normal = rot.rotate(v_normal[i]);
            vertices[i].zinv = 1.0f / vertices[i].cameraPos[Z];
            vertices[i].texturePos = v_txt[i];
        }

        const std::vector<Vec3u>& tri = mesh->getTri();
        const std::vector<Vec3u>& tri_txt = mesh->getTriTxt();
    }

}
