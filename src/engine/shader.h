#pragma once

#include <string>
#include <ramiel/graphics.h>

namespace ramiel {

    class EngineShaderBase {
    public:
        virtual std::string getProp(std::string prop) const = 0;
        virtual void setProp(std::string prop, std::string val) = 0;
    };


    class EngineVertexShaderBase : public EngineShaderBase {
    public:
        virtual VertexShaderBase* get() = 0;
        virtual EngineVertexShaderBase* copy() const = 0;
    };

    class EngineVertexShader : public EngineVertexShaderBase {
    public:
        EngineVertexShader(Vec3f pos, Vec3f rot, Vec3f scale);

        virtual std::string getProp(std::string prop) const override;
        virtual void setProp(std::string prop, std::string val) override;

        virtual VertexShaderBase* get() override;
        virtual EngineVertexShaderBase* copy() const override;

        VertexShader vs;
        Vec3f pos;
        Vec3f rot;
        Vec3f scale;
    };

    class EngineVertexShaderTextured : public EngineVertexShaderBase {
    public:
        EngineVertexShaderTextured(Vec3f pos, Vec3f rot, Vec3f scale);

        virtual std::string getProp(std::string prop) const override;
        virtual void setProp(std::string prop, std::string val) override;

        virtual VertexShaderBase* get() override;
        virtual EngineVertexShaderBase* copy() const override;

        VertexShaderTextured vs;
        Vec3f pos;
        Vec3f rot;
        Vec3f scale;
    };


    class EnginePixelShaderBase : public EngineShaderBase {
    public:
        virtual PixelShaderBase* get() = 0;
        virtual EnginePixelShaderBase* copy() const = 0;
    };

    class EnginePixelShader : public EnginePixelShaderBase {
    public:
        EnginePixelShader(Vec3f color, float specexp, float specint);

        virtual std::string getProp(std::string prop) const override;
        virtual void setProp(std::string prop, std::string val) override;

        virtual PixelShaderBase* get() override;
        virtual EnginePixelShaderBase* copy() const override;

        PixelShader ps;
    };

    class EnginePixelShaderTextured : public EnginePixelShaderBase {
    public:
        EnginePixelShaderTextured(std::string texture, float specexp, float specint);

        virtual std::string getProp(std::string prop) const override;
        virtual void setProp(std::string prop, std::string val) override;

        virtual PixelShaderBase* get() override;
        virtual EnginePixelShaderBase* copy() const override;

        PixelShaderTextured ps;
        std::string texture;
    };

}
