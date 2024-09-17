#pragma once

#include <array>
#include <list>

#include <ramiel/data.h>
#include "camera.h"
#include "vertex.h"
#include "clip.h"
#include "rasterize.h"

namespace ramiel {

    template<class PixelShader, class Vertex>
    void raster(PixelShader& ps, std::array<AsTuple<Vertex>, 3>& v) {
        for (size_t i = 0; i < 3; ++i) {
            Vec4f& vp = vPos(v[i]);
            vp = getProjectionCoord(vp);
            float zinv = 1.0f / vp[W];
            v[i] = v[i] * zinv;
            vp = getScreenCoord(vp);
            vp[W] = zinv;
        }
        
        TriInterpolate2d interpolate(vPos(v[0]), vPos(v[1]), vPos(v[2]));

        rasterizeTri(vPos(v[0]), vPos(v[1]), vPos(v[2]), [&](const Vec2f& pixel) {
            Vec3f weights = interpolate(pixel);
            AsTuple<Vertex> attr = (
                v[0] * weights[0] +
                v[1] * weights[1] +
                v[2] * weights[2]
            );
            attr = attr / vPos(attr)[W];

            size_t i = (size_t)pixel[X] + (size_t)pixel[Y] * getRes()[X];
            if (getDepthBuffer()[i] > vPos(attr)[Z]) {
                getDepthBuffer()[i] = vPos(attr)[Z];
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
        if (!clip(vPos(tri[0]), vPos(tri[1]), vPos(tri[2]), clippedTris)) return;

        ps.init(reinterpret_cast<Vertex*>(&tri));

        if (clippedTris.empty()) {
            raster<PixelShader, Vertex>(ps, tri);
            return;
        }

        TriInterpolate3d interpolate(vPos(tri[0]), vPos(tri[1]), vPos(tri[2]));
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
