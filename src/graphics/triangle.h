#pragma once

#include <array>
#include <forward_list>

#include <ramiel/data.h>
#include "camera.h"
#include "vertex.h"

#define POS template get<0>()

namespace ramiel {

    bool isFrontFacing(const Vec4f& v1, const Vec4f& v2, const Vec4f& v3);


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


    template<class Vertex>
    std::array<AsTuple<Vertex>, 3> clip1(AsTuple<Vertex>& v0, AsTuple<Vertex>& v1, AsTuple<Vertex>& v2) {
        // ratio of line clipped
        float c1 = -v1.POS[Z] / (v0.POS[Z] - v1.POS[Z]);
        float c2 = -v1.POS[Z] / (v2.POS[Z] - v1.POS[Z]);

        // new tri formed from quad
        std::array<AsTuple<Vertex>, 3> tri = {
            v1 + (v0 - v1) * c1,
            v1 + (v2 - v1) * c2,
            v2
        };
        v1 = tri[0];
        return tri;
    }


    template<class Vertex>
    void clip2(AsTuple<Vertex>& v0, AsTuple<Vertex>& v1, AsTuple<Vertex>& v2) {
        // ratio of line clipped
        float c1 = -v0.POS[Z] / (v1.POS[Z] - v0.POS[Z]);
        float c2 = -v2.POS[Z] / (v1.POS[Z] - v2.POS[Z]);

        // clip
        v0 = v0 + (v1 - v0) * c1;
        v2 = v2 + (v1 - v2) * c2;
    }


    template<class Vertex>
    bool clip(std::array<AsTuple<Vertex>, 3>& tri, std::forward_list<std::array<AsTuple<Vertex>, 3>>& moreTris) {
        if (tri[0].POS[Z] < 0.0f) {
            if (tri[1].POS[Z] < 0.0f) {
                if (tri[2].POS[Z] < 0.0f) return false;
                else clip2<Vertex>(tri[1], tri[2], tri[0]);
            }
            else if (tri[2].POS[Z] < 0.0f) clip2<Vertex>(tri[0], tri[1], tri[2]);
            else moreTris.push_front(clip1<Vertex>(tri[2], tri[0], tri[1]));
        }
        else if (tri[1].POS[Z] < 0.0f) {
            if (tri[2].POS[Z] < 0.0f) clip2<Vertex>(tri[2], tri[0], tri[1]);
            else moreTris.push_front(clip1<Vertex>(tri[0], tri[1], tri[2]));
        }
        else if (tri[2].POS[Z] < 0.0f) moreTris.push_front(clip1<Vertex>(tri[1], tri[2], tri[0]));
        return true;
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

        std::forward_list<std::array<AsTuple<Vertex>, 3>> moreTris;
        if (!clip<Vertex>(tri, moreTris)) return;
        
        raster<PixelShader, Vertex>(ps, tri);
        for (auto& t : moreTris) {
            raster<PixelShader, Vertex>(ps, t);
        }
    }

}
