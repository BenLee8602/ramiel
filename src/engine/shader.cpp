#include "shader.h"
#include "command.h"
#include "entity.h"
#include "serialize.h"
using namespace ramiel;

namespace {

    Mat4x4f getWorldTransform(Vec3f pos, Vec3f rot, Vec3f scale) {
        return matmat(matmat(ramiel::scale(scale), rotate(rot)), translate(pos));
    }


    template<class Vs>
    std::string getVsProp(const Vs& vs, std::string prop) {
        if (prop == "pos") return toString(vs.pos);
        if (prop == "rot") return toString(vs.rot);
        if (prop == "scale") return toString(vs.scale);
        return "";
    }

    template<class Ps>
    std::string getPsProp(const Ps& ps, std::string prop) {
        if (prop == "specexp") return toString(ps.specularExponent);
        if (prop == "specint") return toString(ps.specularIntensity);
        return "";
    }


    template<class Vs>
    void setVsProp(Vs& vs, std::string prop, std::string val) {
        if (prop == "pos") fromString(val, vs.pos);
        else if (prop == "rot") fromString(val, vs.rot);
        else if (prop == "scale") fromString(val, vs.scale);
        vs.vs.worldTransform = getWorldTransform(vs.pos, vs.rot, vs.scale);
    }

    template<class Ps>
    void setPsProp(Ps& ps, std::string prop, std::string val) {
        if (prop == "specexp") {
            fromString(val, ps.specularExponent);
        }
        else if (prop == "specint") {
            float specint;
            if (!fromString(val, specint)) return;
            if (specint < 0.0f) return;
            ps.specularIntensity = specint;
        }
    }

}

namespace ramiel {

    EngineVertexShader::EngineVertexShader(
        Vec3f pos,
        Vec3f rot,
        Vec3f scale
    )
        : vs({})
        , pos(pos)
        , rot(rot)
        , scale(scale)
    {
        vs.worldTransform = getWorldTransform(pos, rot, scale);
    }

    std::string EngineVertexShader::getProp(std::string prop) const {
        return getVsProp(*this, prop);
    }

    void EngineVertexShader::setProp(std::string prop, std::string val) {
        setVsProp(*this, prop, val);
    }

    VertexShaderBase* EngineVertexShader::get() {
        return &vs;
    }

    EngineVertexShaderBase* EngineVertexShader::copy() const {
        // attrInPos is changed when linking attributes,
        // so when we try to do trivial copy, we copy
        // the modified values. when we go to link
        // again, it fails.
        //return new EngineVertexShader(*this);
        return new EngineVertexShader(pos, rot, scale);
    }


    EngineVertexShaderTextured::EngineVertexShaderTextured(
        Vec3f pos,
        Vec3f rot,
        Vec3f scale
    )
        : vs({})
        , pos(pos)
        , rot(rot)
        , scale(scale)
    {
        vs.worldTransform = getWorldTransform(pos, rot, scale);
    }

    std::string EngineVertexShaderTextured::getProp(std::string prop) const {
        return getVsProp(*this, prop);
    }

    void EngineVertexShaderTextured::setProp(std::string prop, std::string val) {
        setVsProp(*this, prop, val);
    }

    VertexShaderBase* EngineVertexShaderTextured::get() {
        return &vs;
    }

    EngineVertexShaderBase* EngineVertexShaderTextured::copy() const {
        return new EngineVertexShaderTextured(pos, rot, scale);
    }


    EnginePixelShader::EnginePixelShader(
        Vec3f color,
        float specexp,
        float specint
    )
        : ps(color / 255.0f, specexp, specint, Vec3f{})
    {}

    std::string EnginePixelShader::getProp(std::string prop) const {
        if (prop == "color") return toString(ps.surfaceColor * 255.0f);
        return getPsProp(ps, prop);
    }

    void EnginePixelShader::setProp(std::string prop, std::string val) {
        if (prop != "color") {
            setPsProp(ps, prop, val);
            return;
        }
        Vec3f color;
        if (!fromString(val, color)) return;
        if (color < 0.0f || color > 255.0f) return;
        ps.surfaceColor = color / 255.0f;
    }

    PixelShaderBase* EnginePixelShader::get() {
        return &ps;
    }

    EnginePixelShaderBase* EnginePixelShader::copy() const {
        return new EnginePixelShader(
            ps.surfaceColor * 255,
            ps.specularExponent,
            ps.specularIntensity
        );
    }


    EnginePixelShaderTextured::EnginePixelShaderTextured(
        std::string texturepath,
        float specexp,
        float specint
    )
        : ps({}, {}, {}, {})
    {
        auto texturenode = EngineEntity::cast<EngineTexture>(getTree(texturepath));
        if (!texturenode) return;
        ps = PixelShaderTextured(&texturenode->get(), specexp, specint, Vec3f{});
        texture = texturepath;
    }

    std::string EnginePixelShaderTextured::getProp(std::string prop) const {
        if (prop == "texture") return texture;
        return getPsProp(ps, prop);
    }

    void EnginePixelShaderTextured::setProp(std::string prop, std::string val) {
        if (prop != "texture") {
            setPsProp(ps, prop, val);
            return;
        }
        auto texturenode = EngineEntity::cast<EngineTexture>(getTree(val));
        if (!texturenode) return;
        ps.texture = &texturenode->get();
        texture = val;
    }

    PixelShaderBase* EnginePixelShaderTextured::get() {
        return &ps;
    }

    EnginePixelShaderBase* EnginePixelShaderTextured::copy() const {
        return new EnginePixelShaderTextured(
            texture,
            ps.specularExponent,
            ps.specularIntensity
        );
    }

}
