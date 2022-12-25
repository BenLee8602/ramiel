#pragma once

#include "vertex.h"
#include "light.h"
#include "camera.h"

namespace ramiel {

    class VS_PerTri {
        Camera& camera;
        PhysicsObject* phys;
    public:
        VS_PerTri(
            Camera& camera,
            PhysicsObject* phys = nullptr
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerTri Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = phys->rot.rotate(in.pos) + phys->pos;
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            return out;
        }
    };


    class VS_PerTri_Textured {
        Camera& camera;
        PhysicsObject* phys;
    public:
        VS_PerTri_Textured(
            Camera& camera,
            PhysicsObject* phys = nullptr
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerTri_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = phys->rot.rotate(in.pos) + phys->pos;
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
        Vec3f surfaceColor;
        PhysicsObject* phys;
    public:
        VS_PerVertex(
            Camera& camera,
            LightingListSpecular lightingList,
            Vec3f surfaceColor = vec3f_255,
            PhysicsObject* phys = nullptr
        ) :
            camera(camera),
            lightingList(lightingList),
            surfaceColor(surfaceColor / 255.0f),
            phys(phys)
        {}

        typedef Vertex_PerVertex Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            Vec3f worldPos = phys->rot.rotate(in.pos) + phys->pos;
            out.cameraPos = camera.getCameraCoord(worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.color = surfaceColor * lightingList.getAllLight(worldPos, in.normal);
            return out;
        }
    };


    class VS_PerVertex_Textured {
        Camera& camera;
        LightingListSpecular lightingList;
        PhysicsObject* phys;
    public:
        VS_PerVertex_Textured(
            Camera& camera,
            LightingListSpecular lightingList,
            PhysicsObject* phys = nullptr
        ) :
            camera(camera),
            lightingList(lightingList),
            phys(phys)
        {}

        typedef Vertex_PerVertex_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            Vec3f worldPos = phys->rot.rotate(in.pos) + phys->pos;
            out.cameraPos = camera.getCameraCoord(worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.light = lightingList.getAllLight(worldPos, in.normal);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.texture;
            return out;
        }
    };


    class VS_PerPixel {
        Camera& camera;
        PhysicsObject* phys;
    public:
        VS_PerPixel(
            Camera& camera,
            PhysicsObject* phys = nullptr
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerPixel Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = phys->rot.rotate(in.pos) + phys->pos;
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            return out;
        }
    };


    class VS_PerPixel_Textured {
        Camera& camera;
        PhysicsObject* phys;
    public:
        VS_PerPixel_Textured(
            Camera& camera,
            PhysicsObject* phys = nullptr
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerPixel_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = phys->rot.rotate(in.pos) + phys->pos;
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.texture;
            return out;
        }
    };


    class VS_PerPixel_Smooth {
        Camera& camera;
        PhysicsObject* phys;
    public:
        VS_PerPixel_Smooth(
            Camera& camera,
            PhysicsObject* phys = nullptr
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerPixel_Smooth Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = phys->rot.rotate(in.pos) + phys->pos;
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.normal = in.normal;
            out.zinv = 1.0f / out.cameraPos[Z];
            return out;
        }
    };


    class VS_PerPixel_Smooth_Textured {
        Camera& camera;
        PhysicsObject* phys;
    public:
        VS_PerPixel_Smooth_Textured(
            Camera& camera,
            PhysicsObject* phys = nullptr
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerPixel_Smooth_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            out.worldPos = phys->rot.rotate(in.pos) + phys->pos;
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.screenPos = camera.getScreenCoord(out.cameraPos);
            out.normal = in.normal;
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.texture;
            return out;
        }
    };

}
