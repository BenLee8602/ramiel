#include <iostream>
#include <vector>

#include "shader.h"
#include "token.h"
#include "read.h"
#include "stringify.h"

namespace ramiel::shaderlang {

    TemplateArgs compileVertexShader() {
        auto& shaderName = popToken();
        std::vector<Attr> stateAttrs = readAttrList("STATE");
        std::vector<Attr> inAttrs = readAttrList("IN");
        std::vector<Attr> outAttrs = readAttrList("OUT");

        static const Attr posAttr = { "Vec4", "pos" };
        if (
            outAttrs.empty() ||
            outAttrs[0].type != "Vec4" ||
            outAttrs[0].name != "pos"
        ) {
            std::cout << "error: first vertex shader output must be \"Vec4 pos\"\n";
            exit(1);
        }

        std::string shaderFunc = readShaderFunc("SHADER");

        TemplateArgs templateArgs = {
            { "NAME", shaderName },
            { "STATE", stringifyState(stateAttrs) },
            { "CTOR_ARGS", stringifyCtorArgs(stateAttrs) },
            { "CTOR_INIT", stringifyCtorInit(stateAttrs) },
            { "ATTR_IN_TYPE", stringifyAttrType(inAttrs) },
            { "ATTR_OUT_TYPE", stringifyAttrType(outAttrs) },
            { "ATTR_OUT_POS", stringifyAttrOutPos(outAttrs) },
            { "ATTR_OUT_SIZE", stringifyAttrOutSize(outAttrs) },
            { "ATTR_IN_DEF", stringifyAttrInDef(inAttrs) },
            { "ATTR_OUT_DEF", stringifyAttrOutDef(outAttrs) },
            { "ATTR_IN_INTERPRET", stringifyAttrInInterpret(inAttrs) },
            { "FUNC", shaderFunc }
        };

        return templateArgs;
    }


    TemplateArgs compilePixelShader() {
        auto& shaderName = popToken();
        std::vector<Attr> stateAttrs = readAttrList("STATE");
        std::vector<Attr> inAttrs = readAttrList("IN");

        std::string initFunc = readShaderFunc("INIT");
        std::string shaderFunc = readShaderFunc("SHADER");

        TemplateArgs templateArgs = {
            { "NAME", shaderName },
            { "STATE", stringifyState(stateAttrs) },
            { "CTOR_ARGS", stringifyCtorArgs(stateAttrs) },
            { "CTOR_INIT", stringifyCtorInit(stateAttrs) },
            { "ATTR_IN_TYPE", stringifyAttrType(inAttrs) },
            { "ATTR_IN_DEF", stringifyAttrInDef(inAttrs) },
            { "ATTR_IN_INTERPRET", stringifyAttrInInterpret(inAttrs) },
            { "FUNC_INIT", initFunc },
            { "FUNC", shaderFunc }
        };

        return templateArgs;
    }

}
