#include <iostream>
#include <vector>

#include "pixelshader.h"
#include "attribute.h"
#include "func.h"

namespace ramiel::shaderlang {

    void parsePixelShader(const std::string& outPath) {
        std::vector<Attr> stateAttrs = readAttrList("STATE");
        std::vector<Attr> inAttrs = readAttrList("IN");

        std::string initFunc = readShaderFunc("INIT");
        std::string shaderFunc = readShaderFunc("SHADER");


        // todo: write c++ output

        std::cout << "{\n";
        std::cout << "    state: {\n";
        for (auto& attr : stateAttrs) {
            std::cout << "        { type: \"" << attr.type << "\", name: \"" << attr.name << "\" }\n";
        }
        std::cout << "    },\n";
        std::cout << "    in: {\n";
        for (auto& attr : inAttrs) {
            std::cout << "        { type: \"" << attr.type << "\", name: \"" << attr.name << "\" }\n";
        }
        std::cout << "    },\n";
        std::cout << "    init: \"" << initFunc << "\"\n}\n";
        std::cout << "    shader: \"" << shaderFunc << "\"\n}\n";
    }

}
