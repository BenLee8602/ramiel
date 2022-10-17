#pragma once

#include "mesh.h"
#include "physicsobj.h"
#include "pixelshader.h"

#define VERTEX_SHADER_ARGS Mesh* mesh, float scale, PhysicsObj* physicsObj, PixelShader& pixelShader

namespace ramiel {

    typedef void(VertexShader)(VERTEX_SHADER_ARGS);

    void VS_PerTri(VERTEX_SHADER_ARGS);
    void VS_PerTri_Textured(VERTEX_SHADER_ARGS);
    void VS_PerVertex(VERTEX_SHADER_ARGS);
    void VS_PerVertex_Textured(VERTEX_SHADER_ARGS);
    void VS_PerPixel(VERTEX_SHADER_ARGS);
    void VS_PerPixel_Textured(VERTEX_SHADER_ARGS);
    void VS_PerPixel_Smooth(VERTEX_SHADER_ARGS);
    void VS_PerPixel_Smooth_Textured(VERTEX_SHADER_ARGS);

}
