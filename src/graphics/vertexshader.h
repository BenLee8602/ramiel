#pragma once

#include <vertextypes/VtFlat.h>
#include <vertextypes/VtFlatPixel.h>
#include <vertextypes/VtFlatPixelTxt.h>
#include <vertextypes/VtFlatTxt.h>
#include <vertextypes/VtSmooth.h>
#include <vertextypes/VtSmoothPixel.h>
#include <vertextypes/VtSmoothPixelTxt.h>
#include <vertextypes/VtSmoothTxt.h>

#include <ramiel/ramiel.h>
#include "vertex.h"
#include "light.h"
#include "camera.h"

namespace ramiel {

    class VS_PerTri {
        PhysicsObject* phys;
    public:
        VS_PerTri(PhysicsObject* phys) : phys(phys) {}

        using Vertex_Out = example::VtFlat;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;

            // temp, should only get transform once per frame
            out.pos = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });

            out.worldPos = { out.pos[X], out.pos[Y], out.pos[Z] };

            return out;
        }
    };


    class VS_PerTri_Textured {
        PhysicsObject* phys;
    public:
        VS_PerTri_Textured(PhysicsObject* phys) : phys(phys) {}

        using Vertex_Out = example::VtFlatTxt;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            out.pos = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });

            out.worldPos = { out.pos[X], out.pos[Y], out.pos[Z] };
            out.texturePos = in.txt;

            return out;
        }
    };


    class VS_PerVertex {
        PhysicsObject* phys;
        LightingListSpecular lightingList;
        Vec3f surfaceColor;
    public:
        VS_PerVertex(
            PhysicsObject* phys,
            LightingListSpecular lightingList,
            Vec3f surfaceColor = Vec3f{ 255, 255, 255 }
        ) :
            lightingList(lightingList),
            surfaceColor(surfaceColor / 255.0f),
            phys(phys)
        {}

        using Vertex_Out = example::VtSmooth;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            out.pos = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });

            Vec3f worldPos = { out.pos[X], out.pos[Y], out.pos[Z] };
            out.color = surfaceColor * lightingList.getAllLight(worldPos, in.nml);

            return out;
        }
    };


    class VS_PerVertex_Textured {
        PhysicsObject* phys;
        LightingListSpecular lightingList;
    public:
        VS_PerVertex_Textured(
            PhysicsObject* phys,
            LightingListSpecular lightingList
        ) :
            phys(phys),
            lightingList(lightingList)
        {}

        using Vertex_Out = example::VtSmoothTxt;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            out.pos = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });

            Vec3f worldPos = { out.pos[X], out.pos[Y], out.pos[Z] };
            out.light = lightingList.getAllLight(worldPos, in.nml);
            out.texturePos = in.txt;

            return out;
        }
    };


    class VS_PerPixel {
        PhysicsObject* phys;
    public:
        VS_PerPixel(PhysicsObject* phys) : phys(phys) {}

        using Vertex_Out = example::VtFlatPixel;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            out.pos = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });

            out.worldPos = { out.pos[X], out.pos[Y], out.pos[Z] };

            return out;
        }
    };


    class VS_PerPixel_Textured {
        PhysicsObject* phys;
    public:
        VS_PerPixel_Textured(PhysicsObject* phys) : phys(phys) {}

        using Vertex_Out = example::VtFlatPixelTxt;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            out.pos = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });

            out.worldPos = { out.pos[X], out.pos[Y], out.pos[Z] };
            out.texturePos = in.txt;

            return out;
        }
    };


    class VS_PerPixel_Smooth {
        PhysicsObject* phys;
    public:
        VS_PerPixel_Smooth(PhysicsObject* phys) : phys(phys) {}

        using Vertex_Out = example::VtSmoothPixel;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            out.pos = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });

            out.worldPos = { out.pos[X], out.pos[Y], out.pos[Z] };
            out.normal = in.nml;

            return out;
        }
    };


    class VS_PerPixel_Smooth_Textured {
        PhysicsObject* phys;
    public:
        VS_PerPixel_Smooth_Textured(PhysicsObject* phys) : phys(phys) {}

        using Vertex_Out = example::VtSmoothPixelTxt;
        template<class Vertex_In>
        Vertex_Out operator()(const Vertex_In& in) const {
            Vertex_Out out;
            
            // temp, should only get transform once per frame
            out.pos = matvec(phys->getTransform(), { in.pos[X], in.pos[Y], in.pos[Z], 1 });

            out.worldPos = { out.pos[X], out.pos[Y], out.pos[Z] };
            out.normal = in.nml;
            out.texturePos = in.txt;

            return out;
        }
    };

}
