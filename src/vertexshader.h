#pragma once

#include "vertex.h"
#include "light.h"
#include "camera.h"

namespace ramiel {

    class VS_PerTri {
        Camera& camera;
    public:
        typedef Vertex_Out_PerTri Vertex_Out;

        VS_PerTri(Camera& camera) : camera(camera) {}

        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = in.pos;
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            return out;
        }
    };


    class VS_PerTri_Textured {
        Camera& camera;
    public:
        typedef Vertex_Out_PerTri_Textured Vertex_Out;

        VS_PerTri_Textured(Camera& camera) : camera(camera) {}

        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = in.pos;
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.txt;
            return out;
        }
    };


    class VS_PerVertex {
        LightingListSpecular lightingList;
    public:
        typedef Vertex_Out_PerVertex Vertex_Out;

        VS_PerVertex(LightingListSpecular lightingList) : lightingList(lightingList) {}

        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            Vec3f worldPos = in.pos;
            Vec3f normal = in.normal;
            out.cameraPos = camera.getCameraCoord(worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.color = lightingList.getAllLight(worldPos, normal);
            return out;
        }
    };


    class VS_PerVertex_Textured {
        LightingListSpecular lightingList;
    public:
        typedef Vertex_Out_PerVertex_Textured Vertex_Out;

        VS_PerVertex_Textured(LightingListSpecular lightingList) : lightingList(lightingList) {}

        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            Vec3f worldPos = in.pos;
            Vec3f normal = in.normal;
            out.cameraPos = camera.getCameraCoord(worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.color = lightingList.getAllLight(worldPos, normal);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.texture;
            return out;
        }
    };


    class VS_PerPixel {
    public:
        typedef Vertex_Out_PerPixel Vertex_Out;

        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = in.pos;
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            return out;
        }
    };


    class VS_PerPixel_Textured {
    public:
        typedef Vertex_Out_PerPixel_Textured Vertex_Out;

        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = in.pos;
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.texture;
            return out;
        }
    };


    class VS_PerPixel_Smooth {
    public:
        typedef Vertex_Out_PerPixel_Smooth Vertex_Out;

        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = in.pos;
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.normal = in.normal;
            out.zinv = 1.0f / out.cameraPos[Z];
            return out;
        }
    };


    class VS_PerPixel_Smooth_Textured {
    public:
        typedef Vertex_Out_PerPixel_Smooth_Textured Vertex_Out;

        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = in.pos;
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.normal = in.normal;
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.texture;
            return out;
        }
    };

}
