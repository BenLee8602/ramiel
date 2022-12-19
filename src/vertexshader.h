#pragma once

#include "vertex.h"
#include "light.h"
#include "camera.h"

namespace ramiel {

    class VS_PerTri {
        Camera& camera;
    public:
        VS_PerTri(Camera& camera) : camera(camera) {}

        typedef Vertex_Out_PerTri Vertex_Out;
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
        VS_PerTri_Textured(Camera& camera) : camera(camera) {}

        typedef Vertex_Out_PerTri_Textured Vertex_Out;
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


    class VS_PerVertex {
        Camera& camera;
        LightingListSpecular lightingList;
    public:
        VS_PerVertex(
            Camera& camera,
            LightingListSpecular lightingList
        ) :
            camera(camera),
            lightingList(lightingList)
        {}

        typedef Vertex_Out_PerVertex Vertex_Out;
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
        Camera& camera;
        LightingListSpecular lightingList;
    public:
        VS_PerVertex_Textured(
            Camera& camera,
            LightingListSpecular lightingList
        ) :
            camera(camera),
            lightingList(lightingList)
        {}

        typedef Vertex_Out_PerVertex_Textured Vertex_Out;
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
        Camera& camera;
    public:
        VS_PerPixel(Camera& camera) : camera(camera) {}

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
        Camera& camera;
    public:
        VS_PerPixel_Textured(Camera& camera) : camera(camera) {}

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
        Camera& camera;
    public:
        VS_PerPixel_Smooth(Camera& camera) : camera(camera) {}

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
        Camera& camera;
    public:
        VS_PerPixel_Smooth_Textured(Camera& camera) : camera(camera) {}

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
