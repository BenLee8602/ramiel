#pragma once

#include <ramiel/ramiel.h>
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
            PhysicsObject* phys
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerTri Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;

            // temp, should only get transform once per frame
            Vec4f wp4 = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });
            out.worldPos = { wp4[X], wp4[Y], wp4[Z] };

            out.cameraPos = camera.getCameraCoord(out.worldPos);
            return out;
        }
    };


    class VS_PerTri_Textured {
        Camera& camera;
        PhysicsObject* phys;
    public:
        VS_PerTri_Textured(
            Camera& camera,
            PhysicsObject* phys
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerTri_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            Vec4f wp4 = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });
            out.worldPos = { wp4[X], wp4[Y], wp4[Z] };
            
            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.txt;
            return out;
        }
    };


    class VS_PerVertex {
        Camera& camera;
        PhysicsObject* phys;
        LightingListSpecular lightingList;
        Vec3f surfaceColor;
    public:
        VS_PerVertex(
            Camera& camera,
            PhysicsObject* phys,
            LightingListSpecular lightingList,
            Vec3f surfaceColor = Vec3f{ 255, 255, 255 }
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
            
            // temp, should only get transform once per frame
            Vec4f wp4 = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });
            Vec3f worldPos = { wp4[X], wp4[Y], wp4[Z] };

            out.cameraPos = camera.getCameraCoord(worldPos);
            out.color = surfaceColor * lightingList.getAllLight(worldPos, in.nml);
            return out;
        }
    };


    class VS_PerVertex_Textured {
        Camera& camera;
        PhysicsObject* phys;
        LightingListSpecular lightingList;
    public:
        VS_PerVertex_Textured(
            Camera& camera,
            PhysicsObject* phys,
            LightingListSpecular lightingList
        ) :
            camera(camera),
            phys(phys),
            lightingList(lightingList)
        {}

        typedef Vertex_PerVertex_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            Vec4f wp4 = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });
            Vec3f worldPos = { wp4[X], wp4[Y], wp4[Z] };

            out.cameraPos = camera.getCameraCoord(worldPos);
            out.light = lightingList.getAllLight(worldPos, in.nml);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.txt;
            return out;
        }
    };


    class VS_PerPixel {
        Camera& camera;
        PhysicsObject* phys;
    public:
        VS_PerPixel(
            Camera& camera,
            PhysicsObject* phys
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerPixel Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            Vec4f wp4 = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });
            out.worldPos = { wp4[X], wp4[Y], wp4[Z] };

            out.cameraPos = camera.getCameraCoord(out.worldPos);
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
            PhysicsObject* phys
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerPixel_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            Vec4f wp4 = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });
            out.worldPos = { wp4[X], wp4[Y], wp4[Z] };

            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.txt;
            return out;
        }
    };


    class VS_PerPixel_Smooth {
        Camera& camera;
        PhysicsObject* phys;
    public:
        VS_PerPixel_Smooth(
            Camera& camera,
            PhysicsObject* phys
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerPixel_Smooth Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            Vec4f wp4 = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });
            out.worldPos = { wp4[X], wp4[Y], wp4[Z] };

            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.normal = in.nml;
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
            PhysicsObject* phys
        ) :
            camera(camera),
            phys(phys)
        {}

        typedef Vertex_PerPixel_Smooth_Textured Vertex_Out;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            Vec4f wp4 = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });
            out.worldPos = { wp4[X], wp4[Y], wp4[Z] };

            out.cameraPos = camera.getCameraCoord(out.worldPos);
            out.normal = in.nml;
            out.zinv = 1.0f / out.cameraPos[Z];
            out.texturePos = in.txt;
            return out;
        }
    };

}
