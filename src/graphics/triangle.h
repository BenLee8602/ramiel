#pragma once

#include <array>
#include <forward_list>

#include <ramiel/data.h>
#include "camera.h"
#include "vertex.h"

#define POS template get<0>()

namespace ramiel {

    class TriInterpolate3d {
    public:
        TriInterpolate3d(const Vec4f& v0, const Vec4f& v1, const Vec4f& v2);
        Vec3f operator()(const Vec4f& p) const;
    private:
        Vec3f v1;
        Vec3f x;
        Vec3f y;
        Vec3f z;
        float a;
    };

    bool isFrontFacing(const Vec4f& v0, const Vec4f& v1, const Vec4f& v2);

    using Tri = std::array<Vec4f, 3>;
    using TriList = std::forward_list<Tri>;

    bool clip(const Vec4f& v0, const Vec4f& v1, const Vec4f& v2, TriList& clippedTris);


    template<class PixelShader, class Vertex>
    void raster(PixelShader& ps, std::array<AsTuple<Vertex>, 3>& v) {
        for (size_t i = 0; i < 3; ++i) {
            v[i].POS = getScreenCoord(getNormalizedCoord(v[i].POS));

            // temp!! rasterization will be redone soon
            v[i].POS[X] = std::floor(v[i].POS[X]);
            v[i].POS[Y] = std::floor(v[i].POS[Y]);
        }

        ColorIt color = getColorBuffer();
        DepthIt depth = getDepthBuffer();
        
        if (v[0].POS[Y] > v[1].POS[Y]) std::swap(v[0], v[1]);
        if (v[0].POS[Y] > v[2].POS[Y]) std::swap(v[0], v[2]);
        if (v[1].POS[Y] > v[2].POS[Y]) std::swap(v[1], v[2]);

        AsTuple<Vertex> dy1 = (v[2] - v[0]) / (v[2].POS[Y] - v[0].POS[Y]);
        AsTuple<Vertex> dy2 = (v[1] - v[0]) / (v[1].POS[Y] - v[0].POS[Y]);
        AsTuple<Vertex>* dy;
        if (dy1.POS[X] > dy2.POS[X]) {
            std::swap(dy1, dy2);
            dy = &dy1;
        }
        else dy = &dy2;

        int y = std::max<int>(0, v[0].POS[Y]);
        int ymax = std::min<int>(v[1].POS[Y], getRes()[Y]);
        AsTuple<Vertex> sc1 = v[0] + dy1 * (float)(y - v[0].POS[Y]);
        AsTuple<Vertex> sc2 = v[0] + dy2 * (float)(y - v[0].POS[Y]);
        
        auto drawHalf = [&]() {
            for (y; y < ymax; ++y) {
                AsTuple<Vertex> dx = (sc2 - sc1) / (sc2.POS[X] - sc1.POS[X]);

                int x = std::max<int>(0, sc1.POS[X]);
                int xmax = std::min<int>(sc2.POS[X], getRes()[X]);
                AsTuple<Vertex> p = sc1 + dx * (float)(x - sc1.POS[X]);
                size_t i = getRes()[X] * y + x;

                for (x; x < xmax; ++x) {
                    if (p.POS[Z] < depth[i]) {
                        depth[i] = p.POS[Z];
                        color[i] = ps.draw(reinterpret_cast<const Vertex&>(p));
                    }
                    ++i;
                    p = p + dx;
                }
                sc1 = sc1 + dy1;
                sc2 = sc2 + dy2;
            }
        };
        
        drawHalf();

        ymax = std::min<int>(v[2].POS[Y], getRes()[Y]);
        *dy = (v[2] - v[1]) / (v[2].POS[Y] - v[1].POS[Y]);
        sc1 = v[2] - dy1 * (float)(v[2].POS[Y] - y);
        sc2 = v[2] - dy2 * (float)(v[2].POS[Y] - y);
        
        drawHalf();
    }


    template<class PixelShader, class Vertex>
    void draw(
        PixelShader& ps,
        const Vertex& v0,
        const Vertex& v1,
        const Vertex& v2
    ) {
        assertValidVertex<Vertex>();
        std::array<AsTuple<Vertex>, 3> tri = {
            reinterpret_cast<const AsTuple<Vertex>&>(v0),
            reinterpret_cast<const AsTuple<Vertex>&>(v1),
            reinterpret_cast<const AsTuple<Vertex>&>(v2)
        };
        ps.init(reinterpret_cast<Vertex*>(&tri));

        TriList clippedTris;
        if (!clip(tri[0].POS, tri[1].POS, tri[2].POS, clippedTris)) return;

        if (clippedTris.empty()) {
            raster<PixelShader, Vertex>(ps, tri);
            return;
        }

        TriInterpolate3d interpolate(tri[0].POS, tri[1].POS, tri[2].POS);
        for (auto& t : clippedTris) {
            std::array<AsTuple<Vertex>, 3> tri_c;
            for (size_t i = 0; i < 3; ++i) {
                Vec3f weights = interpolate(t[i]);
                tri_c[i] = (
                    tri[0] * weights[0] +
                    tri[1] * weights[1] +
                    tri[2] * weights[2]
                );
            }
            raster<PixelShader, Vertex>(ps, tri_c);
        }
    }

}
