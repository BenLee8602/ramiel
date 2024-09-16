#pragma once

#include <array>
#include <list>

#include <ramiel/data.h>
#include "camera.h"
#include "vertex.h"
#include "clip.h"
#include "rasterize.h"

#define POS template get<0>()

namespace ramiel {

    template<class PixelShader, class Vertex>
    void raster(PixelShader& ps, std::array<AsTuple<Vertex>, 3>& v) {
        for (size_t i = 0; i < 3; ++i) {
            v[i].POS = getScreenCoord(getNormalizedCoord(getProjectionCoord(v[i].POS)));
        }
        
        TriInterpolate2d interpolate(v[0].POS, v[1].POS, v[2].POS);

        rasterizeTri(v[0].POS, v[1].POS, v[2].POS, [&](const Vec2f& pixel) {
            Vec3f weights = interpolate(pixel);
            AsTuple<Vertex> attr = (
                v[0] * weights[0] +
                v[1] * weights[1] +
                v[2] * weights[2]
            );

            size_t i = (size_t)pixel[X] + (size_t)pixel[Y] * getRes()[X];
            if (getDepthBuffer()[i] > attr.POS[Z]) {
                getDepthBuffer()[i] = attr.POS[Z];
                getColorBuffer()[i] = ps.draw(reinterpret_cast<Vertex&>(attr));
            }
        });
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

        TriList clippedTris;
        if (!clip(tri[0].POS, tri[1].POS, tri[2].POS, clippedTris)) return;

        ps.init(reinterpret_cast<Vertex*>(&tri));

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
