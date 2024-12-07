#include <iostream>
#include <vector>

#include "pixelshader.h"
#include "token.h"
#include "attribute.h"
#include "func.h"
#include "stringify.h"

namespace ramiel::shaderlang {

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
