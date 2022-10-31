#pragma once

#include "vertex.h"
#include "texture.h"

namespace ramiel {

    class PixelShader {
    protected:
        uint16_t specularExponent;
        float specularIntensity;
    public:
        Vec3f getAllLights(const Vec3f& pos, const Vec3f& normal) const;
    };

    class PS_PerTri : public PixelShader {
        Vec3f color;
        Vec3f surfaceColor;
    public:
        PS_PerTri& get() { return *this; }
        void init(Vertex_Out_PerTri v[3]);
        Vec3f draw(const Vertex_Out_PerTri& v);
    };

    class PS_PerTri_Textured : public PixelShader {
        Vec3f light;
        Texture* texture;
    public:
        PS_PerTri_Textured& get() { return *this; }
        void init(Vertex_Out_PerTri_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerTri_Textured& v);
    };

    class PS_PerVertex : public PixelShader {
    public:
        PS_PerVertex& get() { return *this; }
        void init(Vertex_Out_PerVertex v[3]);
        Vec3f draw(const Vertex_Out_PerVertex& v);
    };

    class PS_PerVertex_Textured : public PixelShader {
        Texture* texture;
    public:
        PS_PerVertex_Textured& get() { return *this; }
        void init(Vertex_Out_PerVertex_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerVertex_Textured& v);
    };

    class PS_PerPixel : public PixelShader {
        Vec3f surfaceColor;
        Vec3f normal;
    public:
        PS_PerPixel& get() { return *this; }
        void init(Vertex_Out_PerPixel v[3]);
        Vec3f draw(const Vertex_Out_PerPixel& v);
    };

    class PS_PerPixel_Textured : public PixelShader {
        Vec3f normal;
        Texture* texture;
    public:
        PS_PerPixel_Textured& get() { return *this; }
        void init(Vertex_Out_PerPixel_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerPixel_Textured& v);
    };

    class PS_PerPixel_NormalMapped : public PixelShader {
        Vec3f normal;
        Vec3f tangent;
        Vec3f bitangent;
        Texture* normalmap;
        Vec3f surfaceColor;
    public:
        PS_PerPixel_NormalMapped& get() { return *this; }
        void init(Vertex_Out_PerPixel_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerPixel_Textured& v);
    };

    class PS_PerPixel_Textured_NormalMapped : public PixelShader {
        Vec3f normal;
        Vec3f tangent;
        Vec3f bitangent;
        Texture* texture;
        Texture* normalmap;
    public:
        PS_PerPixel_Textured_NormalMapped& get() { return *this; }
        void init(Vertex_Out_PerPixel_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerPixel_Textured& v);
    };

    class PS_PerPixel_Smooth : public PixelShader {
        Vec3f surfaceColor;
    public:
        PS_PerPixel_Smooth& get() { return *this; }
        void init(Vertex_Out_PerPixel_Smooth v[3]);
        Vec3f draw(const Vertex_Out_PerPixel_Smooth& v);
    };

    class PS_PerPixel_Smooth_Textured : public PixelShader {
        Texture* texture;
    public:
        PS_PerPixel_Smooth_Textured& get() { return *this; }
        void init(Vertex_Out_PerPixel_Smooth_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerPixel_Smooth_Textured& v);
    };

}
