#pragma once

#include "vertex.h"
#include "texture.h"

namespace ramiel {

    class PixelShader {
    protected:
        uint16_t specularExponent;
        float specularIntensity;
    };

    class PS_PerTri : public PixelShader {
        Vec3f color;
        Vec3f surfaceColor;
    public:
        bool init(Vertex_PerTri v[3]);
        Vec3f draw(const Vertex_PerTri& v);
    };

    class PS_PerTri_Textured : public PixelShader {
        Vec3f color;
        Texture* texture;
    public:
        bool init(Vertex_PerTri_Textured v[3]);
        Vec3f draw(const Vertex_PerTri_Textured& v);
    };

    class PS_PerVertex : public PixelShader {
    public:
        bool init(Vertex_PerVertex v[3]);
        Vec3f draw(const Vertex_PerVertex& v);
    };

    class PS_PerVertex_Textured : public PixelShader {
        Texture* texture;
    public:
        bool init(Vertex_PerVertex_Textured v[3]);
        Vec3f draw(const Vertex_PerVertex_Textured& v);
    };

    class PS_PerPixel : public PixelShader {
        Vec3f surfaceColor;
        Vec3f normal;
    public:
        bool init(Vertex_PerPixel v[3]);
        Vec3f draw(const Vertex_PerPixel& v);
    };

    class PS_PerPixel_Textured : public PixelShader {
        Vec3f normal;
        Texture* texture;
    public:
        bool init(Vertex_PerPixel_Textured v[3]);
        Vec3f draw(const Vertex_PerPixel_Textured& v);
    };

    class PS_PerPixel_NormalMapped : public PixelShader {
        Vec3f normal;
        Vec3f tangent;
        Vec3f bitangent;
        Texture* normalmap;
        Vec3f surfaceColor;
    public:
        bool init(Vertex_PerPixel_Textured v[3]);
        Vec3f draw(const Vertex_PerPixel_Textured& v);
    };

    class PS_PerPixel_Textured_NormalMapped : public PixelShader {
        Vec3f normal;
        Vec3f tangent;
        Vec3f bitangent;
        Texture* texture;
        Texture* normalmap;
    public:
        bool init(Vertex_PerPixel_Textured v[3]);
        Vec3f draw(const Vertex_PerPixel_Textured& v);
    };

    class PS_PerPixel_Smooth : public PixelShader {
        Vec3f surfaceColor;
    public:
        bool init(Vertex_PerPixel_Smooth v[3]);
        Vec3f draw(const Vertex_PerPixel_Smooth& v);
    };

    class PS_PerPixel_Smooth_Textured : public PixelShader {
        Texture* texture;
    public:
        bool init(Vertex_PerPixel_Smooth_Textured v[3]);
        Vec3f draw(const Vertex_PerPixel_Smooth_Textured& v);
    };

}
