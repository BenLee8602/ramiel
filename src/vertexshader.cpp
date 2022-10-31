#include "vertexshader.h"
#include "graphics.h"
#include "triangle.h"


namespace ramiel {

    template<typename Vertex, class PixelShader>
    static void drawAllTriangles(MeshBase* mesh, const std::vector<Vertex>& vertices, PixelShader& pixelShader) {
        const std::vector<Vec3u>& triList = mesh->getTri();
        for (size_t i = 0; i < triList.size(); ++i) {
            // backface culling
            if (dotProduct(
                crossProduct(
                    vertices[triList[i][1]].cameraPos - vertices[triList[i][0]].cameraPos,
                    vertices[triList[i][2]].cameraPos - vertices[triList[i][0]].cameraPos
                ), v[0].cameraPos
            ) >= 0.0f) continue;

            // assemble and draw triangle
            Triangle tri = {
                pixelShader,
                vertices[triList[i][0]],
                vertices[triList[i][1]],
                vertices[triList[i][2]]
            };
            triangle.draw();
        }
    }


    Vertex_Out_PerTri VS_PerTri::operator()(const Vertex_In_P& in) const {
        Vertex_Out_PerTri out;
        out.worldPos = phys.getRot().rotate(in.pos) * scale + phys.getPos();
        out.cameraPos = graphics::camera.getCameraCoord(out.worldPos);
        out.screenPos = graphics::camera.getScreenCoord(out.cameraPos);
        return out;
    }


    Vertex_Out_PerTri_Textured VS_PerTri_Textured::operator()(const Vertex_In_PT& in) const {
        Vertex_Out_PerTri_Textured out;
        out.worldPos = phys.getRot().rotate(in.pos) * scale + phys.getPos();
        out.cameraPos = graphics::camera.getCameraCoord(out.worldPos);
        out.screenPos = graphics::camera.getScreenCoord(out.cameraPos);
        out.zinv = 1.0f / out.cameraPos[Z];
        out.texturePos = in.txt;
        return out;
    }

    
    Vertex_Out_PerVertex VS_PerVertex::operator()(const Vertex_In_PN& in) const {
        Vertex_Out_PerVertex out;
        Vec3f worldPos = phys.getRot().rotate(in.pos) * scale + phys.getPos();
        Vec3f normal = phys.getRot().rotate(in.nml);
        out.cameraPos = graphics::camera.getCameraCoord(worldPos);
        out.screenPos = graphics::camera.getScreenCoord(out.cameraPos);
        //out.color = pixelShader.getAllLights(worldPos, normal);
        return out;
    }

    
    Vertex_Out_PerVertex_Textured VS_PerVertex_Textured::operator()(const Vertex_In_PNT& in) const {
        Vertex_Out_PerVertex_Textured out;
        Vec3f worldPos = phys.getRot().rotate(in.pos) * scale + phys.getPos();
        Vec3f normal = phys.getRot().rotate(in.nml);
        out.cameraPos = graphics::camera.getCameraCoord(worldPos);
        out.screenPos = graphics::camera.getScreenCoord(out.cameraPos);
        //out.color = pixelShader.getAllLights(worldPos, normal);
        out.zinv = 1.0f / out.cameraPos[Z];
        out.texturePos = in.txt;
        return out;
    }

    
    Vertex_Out_PerPixel VS_PerPixel::operator()(const Vertex_In_P& in) const {
        Vertex_Out_PerPixel out;
        out.worldPos = phys.getRot().rotate(in.pos) * scale + phys.getPos();
        out.cameraPos = graphics::camera.getCameraCoord(out.worldPos);
        out.screenPos = graphics::camera.getScreenCoord(out.cameraPos);
        out.zinv = 1.0f / out.cameraPos[Z];
        return out;
    }

    
    Vertex_Out_PerPixel_Textured VS_PerPixel_Textured::operator()(const Vertex_In_PT& in) const {
        Vertex_Out_PerPixel_Textured out;
        out.worldPos = phys.getRot().rotate(in.pos) * scale + phys.getPos();
        out.cameraPos = graphics::camera.getCameraCoord(out.worldPos);
        out.screenPos = graphics::camera.getScreenCoord(out.cameraPos);
        out.zinv = 1.0f / out.cameraPos[Z];
        out.texturePos = in.txt;
        return out;
    }

    
    Vertex_Out_PerPixel_Smooth VS_PerPixel_Smooth::operator()(const Vertex_In_PN& in) const {
        Vertex_Out_PerPixel_Smooth out;
        out.worldPos = phys.getRot().rotate(in.pos) * scale + phys.getPos();
        out.cameraPos = graphics::camera.getCameraCoord(out.worldPos);
        out.screenPos = graphics::camera.getScreenCoord(out.cameraPos);
        out.normal = phys.getRot().rotate(in.nml);
        out.zinv = 1.0f / out.cameraPos[Z];
        return out;
    }

    
    Vertex_Out_PerPixel_Smooth_Textured VS_PerPixel_Smooth_Textured::operator()(const Vertex_In_PNT& in) const {
        Vertex_Out_PerPixel_Smooth_Textured out;
        out.worldPos = phys.getRot().rotate(in.pos) * scale + phys.getPos();
        out.cameraPos = graphics::camera.getCameraCoord(out.worldPos);
        out.screenPos = graphics::camera.getScreenCoord(out.cameraPos);
        out.normal = phys.getRot().rotate(in.nml);
        out.zinv = 1.0f / out.cameraPos[Z];
        out.texturePos = in.txt;
        return out;
    }

}
