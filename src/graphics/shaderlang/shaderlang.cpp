#include <iostream>
#include <fstream>

#include "shaderlang.h"
#include "token.h"
#include "attribute.h"
#include "func.h"

namespace {

    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cout << "error: unable to open shader source (" << filename << ")\n";
            exit(1);
        }

        return std::string(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()
        );
    }

}

namespace ramiel::shaderlang {

    void compileShader(
        const std::string& inputFilename,
        const std::string& outputDirectory
    ) {
        tokenize(readFile(inputFilename));

        auto& fileSig = popToken();
        if (fileSig != "RAMIEL") {
            std::cout << "error: expected \"RAMIEL\", got \"" << fileSig << "\"\n";
            exit(1);
        }

        auto& shaderType = popToken();
        if (shaderType != "VERTEX_SHADER" && shaderType != "PIXEL_SHADER") {
            std::cout << "error: invalid shader type \"" << shaderType << "\"\n";
            exit(1);
        }

        std::vector<Attr> stateAttrs;
        std::vector<Attr> inAttrs;
        std::vector<Attr> outAttrs;

        stateAttrs = readAttrList("STATE");
        inAttrs = readAttrList("IN");
        if (shaderType != "PIXEL_SHADER") {
            outAttrs = readAttrList("OUT");
        }

        std::string shaderFunc = readShaderFunc();


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
        if (shaderType != "PIXEL_SHADER") {
            std::cout << "    out: {\n";
            for (auto& attr : outAttrs) {
                std::cout << "        { type: \"" << attr.type << "\", name: \"" << attr.name << "\" }\n";
            }
            std::cout << "    },\n";
        }
        std::cout << "    func: \"" << shaderFunc << "\"\n}\n";
    }

}
