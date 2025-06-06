#pragma once

#include <string>
#include <ramiel/graphics.h>

namespace ramiel {

    class EngineShaderBase {
    public:
        virtual std::string getProp(std::string prop) const = 0;
        virtual void setProp(std::string prop, std::string val) = 0;
        virtual void serialize(BinaryWriter& file) const = 0;
    };


    class EngineVertexShaderBase : public EngineShaderBase {
    public:
        static EngineVertexShaderBase* make(BinaryReader& file);
        virtual VertexShaderBase* get() = 0;
        virtual EngineVertexShaderBase* copy() const = 0;
        virtual void setTransform(Mat4x4f transform) = 0;
    };

    class EngineVertexShader : public EngineVertexShaderBase {
    public:
        EngineVertexShader(Vec3f pos, Vec3f rot, Vec3f scale);
        EngineVertexShader(BinaryReader& file);

        virtual std::string getProp(std::string prop) const override;
        virtual void setProp(std::string prop, std::string val) override;

        virtual VertexShaderBase* get() override;
        virtual EngineVertexShaderBase* copy() const override;
        virtual void setTransform(Mat4x4f transform) override;
        virtual void serialize(BinaryWriter& file) const override;

        VertexShader vs;
        Vec3f scale;
    };

    class EngineVertexShaderTextured : public EngineVertexShaderBase {
    public:
        EngineVertexShaderTextured(Vec3f pos, Vec3f rot, Vec3f scale);
        EngineVertexShaderTextured(BinaryReader& file);

        virtual std::string getProp(std::string prop) const override;
        virtual void setProp(std::string prop, std::string val) override;

        virtual VertexShaderBase* get() override;
        virtual EngineVertexShaderBase* copy() const override;
        virtual void setTransform(Mat4x4f transform) override;
        virtual void serialize(BinaryWriter& file) const override;

        VertexShaderTextured vs;
        Vec3f scale;
    };


    class EnginePixelShaderBase : public EngineShaderBase {
    public:
        static EnginePixelShaderBase* make(BinaryReader& file);
        virtual PixelShaderBase* get() = 0;
        virtual EnginePixelShaderBase* copy() const = 0;
    };

    class EnginePixelShader : public EnginePixelShaderBase {
    public:
        EnginePixelShader(Vec3f color, float specexp, float specint);
        EnginePixelShader(BinaryReader& file);

        virtual std::string getProp(std::string prop) const override;
        virtual void setProp(std::string prop, std::string val) override;

        virtual PixelShaderBase* get() override;
        virtual EnginePixelShaderBase* copy() const override;
        virtual void serialize(BinaryWriter& file) const override;

        PixelShader ps;
    };

    class EnginePixelShaderTextured : public EnginePixelShaderBase {
    public:
        EnginePixelShaderTextured(Tree::H texture, float specexp, float specint);
        EnginePixelShaderTextured(BinaryReader& file);

        virtual std::string getProp(std::string prop) const override;
        virtual void setProp(std::string prop, std::string val) override;

        virtual PixelShaderBase* get() override;
        virtual EnginePixelShaderBase* copy() const override;
        virtual void serialize(BinaryWriter& file) const override;

        PixelShaderTextured ps;
        Tree::H texture;
    };

}
