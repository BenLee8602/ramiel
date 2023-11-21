#pragma once

#include <ramiel/math.h>
#include "vertex.h"
#include "light.h"
#include "camera.h"

namespace ramiel {

    class VS_PerTri {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerTri(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerTri Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            return out;
        }
    };


    class VS_PerTri_Textured {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerTri_Textured(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerTri_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.txt;
            return out;
        }
    };


    class VS_PerVertex {
        Camera& camera;
        Transform* transform;
        LightingListSpecular lightingList;
        Vec3f surfaceColor;
    public:
        VS_PerVertex(
            Camera& camera,
            Transform* transform,
            LightingListSpecular lightingList,
            Vec3f surfaceColor = vec3f_255
        ) :
            camera(camera),
            lightingList(lightingList),
            surfaceColor(surfaceColor / 255.0f),
            transform(transform)
        {}

        typedef Vertex_PerVertex Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            Vec3f worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(worldPos);
            out.color = surfaceColor * lightingList.getAllLight(worldPos, in.nml);
            return out;
        }
    };


    class VS_PerVertex_Textured {
        Camera& camera;
        Transform* transform;
        LightingListSpecular lightingList;
    public:
        VS_PerVertex_Textured(
            Camera& camera,
            Transform* transform,
            LightingListSpecular lightingList
        ) :
            camera(camera),
            transform(transform),
            lightingList(lightingList)
        {}

        typedef Vertex_PerVertex_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            Vec3f worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(worldPos);
            out.light = lightingList.getAllLight(worldPos, in.nml);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.txt;
            return out;
        }
    };


    class VS_PerPixel {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerPixel(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerPixel Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            return out;
        }
    };


    class VS_PerPixel_Textured {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerPixel_Textured(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerPixel_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.txt;
            return out;
        }
    };


    class VS_PerPixel_Smooth {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerPixel_Smooth(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerPixel_Smooth Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.normal = in.nml;
            out.zinv = 1.0f / out.cameraPos[Z];
            return out;
        }
    };


    class VS_PerPixel_Smooth_Textured {
        Camera& camera;
        Transform* transform;
    public:
        VS_PerPixel_Smooth_Textured(
            Camera& camera,
            Transform* transform
        ) :
            camera(camera),
            transform(transform)
        {}

        typedef Vertex_PerPixel_Smooth_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = (*transform)(in.pos);
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.normal = in.nml;
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.txt;
            return out;
        }
    };

}
