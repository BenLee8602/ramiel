#include <iostream>
#include <vector>

#include "vertexshader.h"
#include "attribute.h"
#include "func.h"

namespace ramiel::shaderlang {

    void parseVertexShader(const std::string& outPath) {
        std::vector<Attr> stateAttrs = readAttrList("STATE");
        std::vector<Attr> inAttrs = readAttrList("IN");
        std::vector<Attr> outAttrs = readAttrList("OUT");

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
        std::cout << "    out: {\n";
        for (auto& attr : outAttrs) {
            std::cout << "        { type: \"" << attr.type << "\", name: \"" << attr.name << "\" }\n";
        }
        std::cout << "    },\n";
        std::cout << "    func: \"" << shaderFunc << "\"\n}\n";
    }

}
