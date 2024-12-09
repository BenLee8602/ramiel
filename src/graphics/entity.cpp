#include "entity.h"
#include "vertexshader.h"
#include "pixelshader.h"
#include "clip.h"
#include "rasterize.h"
using namespace ramiel;

namespace {

    Vec4f& vpos(float* v) {
        return *reinterpret_cast<Vec4f*>(v);
    }

    const Vec4f& vpos(const float* v) {
        return *reinterpret_cast<const Vec4f*>(v);
    }


    void camToScreen(float* v, const uint32_t vSize) {
        Vec4f& vp = vpos(v);
        vp = getProjectionCoord(vp);
        float zinv = 1.0f / vp[W];
        for (size_t i = 0; i < vSize; i++) {
            v[i] *= zinv;
        }
        vp = getScreenCoord(vp);
        vp[W] = zinv;
    }


    void rasterize(
        const std::unique_ptr<PixelShaderBase>& ps,
        const uint32_t vSize,
        float* v0,
        float* v1,
        float* v2
    ) {
        camToScreen(v0, vSize);
        camToScreen(v1, vSize);
        camToScreen(v2, vSize);

        std::vector<float> v(vSize);
        TriInterpolate2d interpolate(vpos(v0), vpos(v1), vpos(v2));

        rasterizeTri(vpos(v0), vpos(v1), vpos(v2), [&](const Vec2f& pixel) {
            Vec3f weights = interpolate(pixel);

            for (size_t i = 0; i < vSize; i++) {
                v[i] = (
                    v0[i] * weights[0] +
                    v1[i] * weights[1] +
                    v2[i] * weights[2]
                );
            }

            float z = 1.0f / v[W];
            for (size_t i = 0; i < vSize; i++) {
                v[i] *= z;
            }

            size_t i = (size_t)pixel[X] + (size_t)pixel[Y] * getRes()[X];
            if (getDepthBuffer()[i] > v[Z]) {
                getDepthBuffer()[i] = v[Z];
                getColorBuffer()[i] = ps->run(v.data());
            }
        });
    }


    void rasterize(
        const std::unique_ptr<PixelShaderBase>& ps,
        const uint32_t vSize,
        const float* v0,
        const float* v1,
        const float* v2
    ) {
        std::vector<float> vCopy(vSize * 3);

        std::copy(v0, v0 + vSize, vCopy.data());
        std::copy(v1, v1 + vSize, vCopy.data() + vSize);
        std::copy(v2, v2 + vSize, vCopy.data() + vSize * 2);

        rasterize(
            ps,
            vSize,
            vCopy.data(),
            vCopy.data() + vSize,
            vCopy.data() + vSize * 2
        );
    }

}

namespace ramiel {

    Entity::Entity(
        std::shared_ptr<Mesh> mesh,
        std::unique_ptr<VertexShaderBase>&& vertexShader,
        std::unique_ptr<PixelShaderBase>&& pixelShader
    ) :
        mesh(nullptr),
        vertexShader(nullptr),
        pixelShader(nullptr)
    {
        auto meshAttrOutType = mesh->getAttrOutType();
        auto meshAttrOutPos = mesh->getAttrOutPos();
        auto vsAttrInType = vertexShader->getAttrInType();
        auto& vsAttrInPos = vertexShader->getAttrInPos();

        auto vsAttrOutType = vertexShader->getAttrOutType();
        auto vsAttrOutPos = vertexShader->getAttrOutPos();
        auto psAttrInType = pixelShader->getAttrInType();
        auto& psAttrInPos = pixelShader->getAttrInPos();

        for (size_t i = 0; i < vsAttrInType.size(); i++) {
            if (vsAttrInType[i] != meshAttrOutType[vsAttrInPos[i]]) {
                return;
            }
        }

        for (size_t i = 0; i < psAttrInType.size(); i++) {
            if (psAttrInType[i] != vsAttrOutType[psAttrInPos[i]]) {
                return;
            }
        }

        for (auto& pos : vsAttrInPos) {
            pos = meshAttrOutPos[pos];
        }

        for (auto& pos : psAttrInPos) {
            pos = vsAttrOutPos[pos];
        }

        this->mesh = mesh;
        this->vertexShader = std::move(vertexShader);
        this->pixelShader = std::move(pixelShader);
    }


    Entity::operator bool() {
        return mesh && vertexShader && pixelShader;
    }


    void Entity::draw() {
        if (!(*this)) return;

        const size_t vInSize = mesh->getAttrOutSize();
        const std::vector<float>& vIn = mesh->getVertices();

        const size_t vCount = vIn.size() / vInSize;

        const size_t vSize = vertexShader->getAttrOutSize();
        std::vector<float> v(vSize * vCount);

        for (size_t i = 0; i < vCount; i++) {
            vertexShader->run(&vIn[i * vInSize], &v[i * vSize]);
        }
        
        const std::vector<uint32_t>& tris = mesh->getTriangles();
        for (auto t = tris.begin(); t < tris.end(); t += 3) {
            const float* v0 = &v[t[0] * vSize];
            const float* v1 = &v[t[1] * vSize];
            const float* v2 = &v[t[2] * vSize];

            TriList clippedTris;
            if (!clip(vpos(v0), vpos(v1), vpos(v2), clippedTris)) continue;

            pixelShader->init(v0, v1, v2);

            if (clippedTris.empty()) {
                rasterize(pixelShader, vSize, v0, v1, v2);
                continue;
            }

            std::vector<float> tri(3 * vSize);
            TriInterpolate3d interpolate(vpos(v0), vpos(v1), vpos(v2));
            for (auto& t : clippedTris) {
                for (size_t i = 0; i < 3; i++) {
                    Vec3f weights = interpolate(t[i]);
                    for (size_t j = 0; j < vSize; j++) {
                        tri[vSize * i + j] = (
                            v0[j] * weights[0] +
                            v1[j] * weights[1] +
                            v2[j] * weights[2]
                        );
                    }
                }
                rasterize(
                    pixelShader,
                    vSize,
                    tri.data(),
                    tri.data() + vSize,
                    tri.data() + vSize * 2
                );
            }
        }
    }

}
