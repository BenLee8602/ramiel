#pragma once

#include "physicsobj.h"

#define RAMIEL_VERTEXSHADER_PIPELINE_IMPL virtual void pipeline() const override {  }

namespace ramiel {

    class VertexShader {
    public:
        virtual void pipeline() const = 0;
    };

    class VS_Transformed : public VertexShader {
    protected:
        const PhysicsObj& phys;
        float scale;
    public:
        VS_Transformed(const PhysicsObj& phys, float scale) : phys(phys), scale(scale) {}
    };

    class VS_PerTri : public VS_Transformed {
    public:
        VS_PerTri(const PhysicsObj& phys, float scale) : VS_Transformed(phys, scale) {}
        Vertex_Out_PerTri operator()(const Vertex_In_P& in) const;
        RAMIEL_VERTEXSHADER_PIPELINE_IMPL
    };

    class VS_PerTri_Textured : public VS_Transformed {
    public:
        VS_PerTri_Textured(const PhysicsObj& phys, float scale) : VS_Transformed(phys, scale) {}
        Vertex_Out_PerTri_Textured operator()(const Vertex_In_PT& in) const;
        RAMIEL_VERTEXSHADER_PIPELINE_IMPL
    };

    class VS_PerVertex : public VS_Transformed {

    public:
        VS_PerVertex(const PhysicsObj& phys, float scale) : VS_Transformed(phys, scale) {}
        Vertex_Out_PerVertex operator()(const Vertex_In_PN& in) const;
        RAMIEL_VERTEXSHADER_PIPELINE_IMPL
    };

    class VS_PerVertex_Textured : public VS_Transformed {
        
    public:
        VS_PerVertex_Textured(const PhysicsObj& phys, float scale) : VS_Transformed(phys, scale) {}
        Vertex_Out_PerVertex_Textured operator()(const Vertex_In_PNT& in) const;
        RAMIEL_VERTEXSHADER_PIPELINE_IMPL
    };

    class VS_PerPixel : public VS_Transformed {
    public:
        VS_PerPixel(const PhysicsObj& phys, float scale) : VS_Transformed(phys, scale) {}
        Vertex_Out_PerPixel operator()(const Vertex_In_P& in) const;
        RAMIEL_VERTEXSHADER_PIPELINE_IMPL
    };

    class VS_PerPixel_Textured : public VS_Transformed {
    public:
        VS_PerPixel_Textured(const PhysicsObj& phys, float scale) : VS_Transformed(phys, scale) {}
        Vertex_Out_PerPixel_Textured operator()(const Vertex_In_PT& in) const;
        RAMIEL_VERTEXSHADER_PIPELINE_IMPL
    };

    class VS_PerPixel_Smooth : public VS_Transformed {
    public:
        VS_PerPixel_Smooth(const PhysicsObj& phys, float scale) : VS_Transformed(phys, scale) {}
        Vertex_Out_PerPixel_Smooth operator()(const Vertex_In_PN& in) const;
        RAMIEL_VERTEXSHADER_PIPELINE_IMPL
    };

    class VS_PerPixel_Smooth_Textured : public VS_Transformed {
    public:
        VS_PerPixel_Smooth_Textured(const PhysicsObj& phys, float scale) : VS_Transformed(phys, scale) {}
        Vertex_Out_PerPixel_Smooth_Textured operator()(const Vertex_In_PNT& in) const;
        RAMIEL_VERTEXSHADER_PIPELINE_IMPL
    };

}
