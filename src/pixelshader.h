#pragma once

#include "vertex.h"
#include "texture.h"
#include "light.h"

namespace ramiel {

    class PS_PerTri {
        LightingListSpecular lightingList;
        Vec3f surfaceColor;
        Vec3f color;
    public:
        PS_PerTri(
            LightingListSpecular lightingList,
            Vec3f surfaceColor
        ) :
            lightingList(lightingList),
            surfaceColor(surfaceColor / 255.0f),
            color(vec3f_0)
        {}
        void init(Vertex_Out_PerTri v[3]);
        Vec3f draw(const Vertex_Out_PerTri& v);
    };

    class PS_PerTri_Textured {
        LightingListSpecular lightingList;
        const Texture* texture;
        Vec3f light;
    public:
        PS_PerTri_Textured(
            LightingListSpecular lightingList,
            const Texture* texture
        ) :
            lightingList(lightingList),
            texture(texture),
            light(vec3f_0)
        {}
        void init(Vertex_Out_PerTri_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerTri_Textured& v);
    };

    class PS_PerVertex {
    public:
        void init(Vertex_Out_PerVertex v[3]);
        Vec3f draw(const Vertex_Out_PerVertex& v);
    };

    class PS_PerVertex_Textured {
        const Texture* texture;
    public:
        PS_PerVertex_Textured(const Texture* texture) : texture(texture) {}
        void init(Vertex_Out_PerVertex_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerVertex_Textured& v);
    };

    class PS_PerPixel {
        LightingListSpecular lightingList;
        Vec3f surfaceColor;
        Vec3f normal;
    public:
        PS_PerPixel(
            LightingListSpecular lightingList,
            Vec3f surfaceColor
        ) :
            lightingList(lightingList),
            surfaceColor(surfaceColor / 255.0f),
            normal(vec3f_0)
        {}
        void init(Vertex_Out_PerPixel v[3]);
        Vec3f draw(const Vertex_Out_PerPixel& v);
    };

    class PS_PerPixel_Textured {
        LightingListSpecular lightingList;
        const Texture* texture;
        Vec3f normal;
    public:
        PS_PerPixel_Textured(
            LightingListSpecular lightingList,
            const Texture* texture
        ) :
            lightingList(lightingList),
            texture(texture),
            normal(vec3f_0)
        {}
        void init(Vertex_Out_PerPixel_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerPixel_Textured& v);
    };

    class PS_PerPixel_NormalMapped {
        LightingListSpecular lightingList;
        const Texture* normalMap;
        Vec3f surfaceColor;
        Vec3f normal;
        Vec3f tangent;
        Vec3f bitangent;
    public:
        PS_PerPixel_NormalMapped(
            LightingListSpecular lightingList,
            const Texture* normalMap,
            Vec3f surfaceColor
        ) :
            lightingList(lightingList),
            normalMap(normalMap),
            surfaceColor(surfaceColor / 255.0f),
            normal(vec3f_0),
            tangent(vec3f_0),
            bitangent(vec3f_0)
        {}
        void init(Vertex_Out_PerPixel_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerPixel_Textured& v);
    };

    class PS_PerPixel_Textured_NormalMapped {
        LightingListSpecular lightingList;
        const Texture* texture;
        const Texture* normalMap;
        Vec3f normal;
        Vec3f tangent;
        Vec3f bitangent;
    public:
        PS_PerPixel_Textured_NormalMapped(
            LightingListSpecular lightingList,
            const Texture* texture,
            const Texture* normalMap
        ) :
            lightingList(lightingList),
            texture(texture),
            normalMap(normalMap),
            normal(vec3f_0),
            tangent(vec3f_0),
            bitangent(vec3f_0)
        {}
        void init(Vertex_Out_PerPixel_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerPixel_Textured& v);
    };

    class PS_PerPixel_Smooth {
        LightingListSpecular lightingList;
        Vec3f surfaceColor;
    public:
        PS_PerPixel_Smooth(
            LightingListSpecular lightingList,
            Vec3f surfaceColor
        ) :
            lightingList(lightingList),
            surfaceColor(surfaceColor / 255.0f)
        {}
        void init(Vertex_Out_PerPixel_Smooth v[3]);
        Vec3f draw(const Vertex_Out_PerPixel_Smooth& v);
    };

    class PS_PerPixel_Smooth_Textured {
        LightingListSpecular lightingList;
        const Texture* texture;
    public:
        PS_PerPixel_Smooth_Textured(
            LightingListSpecular lightingList,
            const Texture* texture
        ) :
            lightingList(lightingList),
            texture(texture)
        {}
        void init(Vertex_Out_PerPixel_Smooth_Textured v[3]);
        Vec3f draw(const Vertex_Out_PerPixel_Smooth_Textured& v);
    };

}
