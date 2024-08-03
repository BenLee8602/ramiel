#pragma once

#include <ramiel/data.h>
#include "camera.h"
#include "vertex.h"

#define POS template get<0>()

namespace ramiel {

    bool isFrontFacing(const Vec4f& v1, const Vec4f& v2, const Vec4f& v3);


    template<typename Vertex, class PixelShader>
    class Triangle {
        using VtTuple = AsTuple<Vertex>;
        VtTuple v[3];
        PixelShader pixelShader;

        public:
        Triangle(PixelShader pixelShader) : pixelShader(pixelShader) {}


        private:
        void raster() {
            for (size_t i = 0; i < 3; ++i) {
                v[i].POS = getScreenCoord(getNormalizedCoord(v[i].POS));
            }

            ColorIt color = getColorBuffer();
            DepthIt depth = getDepthBuffer();
            
            if (v[0].POS[Y] > v[1].POS[Y]) std::swap(v[0], v[1]);
            if (v[0].POS[Y] > v[2].POS[Y]) std::swap(v[0], v[2]);
            if (v[1].POS[Y] > v[2].POS[Y]) std::swap(v[1], v[2]);

            VtTuple dy1 = (v[2] - v[0]) / (v[2].POS[Y] - v[0].POS[Y]);
            VtTuple dy2 = (v[1] - v[0]) / (v[1].POS[Y] - v[0].POS[Y]);
            VtTuple* dy;
            if (dy1.POS[X] > dy2.POS[X]) {
                std::swap(dy1, dy2);
                dy = &dy1;
            }
            else dy = &dy2;

            int y = std::max<int>(0, v[0].POS[Y]);
            int ymax = std::min<int>(v[1].POS[Y], getRes()[Y]);
            VtTuple sc1 = v[0] + dy1 * (float)(y - v[0].POS[Y]);
            VtTuple sc2 = v[0] + dy2 * (float)(y - v[0].POS[Y]);
            
            auto drawHalf = [&]() {
                for (y; y < ymax; ++y) {
                    VtTuple dx = (sc2 - sc1) / (sc2.POS[X] - sc1.POS[X]);

                    int x = std::max<int>(0, sc1.POS[X]);
                    int xmax = std::min<int>(sc2.POS[X], getRes()[X]);
                    VtTuple p = sc1 + dx * (float)(x - sc1.POS[X]);
                    size_t i = getRes()[X] * y + x;

                    for (x; x < xmax; ++x) {
                        if (p.POS[Z] < depth[i]) {
                            depth[i] = p.POS[Z];
                            color[i] = pixelShader.draw(reinterpret_cast<const Vertex&>(p));
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


        private:
        void clip1(VtTuple& v0, VtTuple& v1, VtTuple& v2) {
            // ratio of line clipped
            float c1 = -v1.POS[Z] / (v0.POS[Z] - v1.POS[Z]);
            float c2 = -v1.POS[Z] / (v2.POS[Z] - v1.POS[Z]);

            // new tri formed from quad
            Triangle newtri = Triangle(pixelShader);

            // clip and raster new tri
            newtri.v[0] = v1 + (v0 - v1) * c1;
            newtri.v[1] = v1 + (v2 - v1) * c2;
            newtri.v[2] = v2;
            v1 = newtri.v[0];

            newtri.raster();
        }


        private:
        void clip2(VtTuple& v0, VtTuple& v1, VtTuple& v2) {
            // ratio of line clipped
            float c1 = -v0.POS[Z] / (v1.POS[Z] - v0.POS[Z]);
            float c2 = -v2.POS[Z] / (v1.POS[Z] - v2.POS[Z]);

            // clip
            v0 = v0 + (v1 - v0) * c1;
            v2 = v2 + (v1 - v2) * c2;
        }


        private:
        bool clip() {
            if (v[0].POS[Z] < 0.0f) {
                if (v[1].POS[Z] < 0.0f) {
                    if (v[2].POS[Z] < 0.0f) return false;
                    else clip2(v[1], v[2], v[0]);
                }
                else if (v[2].POS[Z] < 0.0f) clip2(v[0], v[1], v[2]);
                else clip1(v[2], v[0], v[1]);
            }
            else if (v[1].POS[Z] < 0.0f) {
                if (v[2].POS[Z] < 0.0f) clip2(v[2], v[0], v[1]);
                else clip1(v[0], v[1], v[2]);
            }
            else if (v[2].POS[Z] < 0.0f) clip1(v[1], v[2], v[0]);
            return true;
        }


        public:
        void draw(
            const Vertex& v0,
            const Vertex& v1,
            const Vertex& v2
        ) {
            assertValidVertex<Vertex>();
            v[0] = reinterpret_cast<const VtTuple&>(v0);
            v[1] = reinterpret_cast<const VtTuple&>(v1);
            v[2] = reinterpret_cast<const VtTuple&>(v2);
            pixelShader.init(reinterpret_cast<Vertex*>(v));
            if (clip()) raster();
        }

    };

}
