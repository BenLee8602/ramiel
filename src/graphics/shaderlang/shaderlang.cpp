#include <iostream>
#include <fstream>

#include "shaderlang.h"
#include "token.h"
#include "attribute.h"
#include "func.h"

#include "vertexshader.h"
#include "pixelshader.h"

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
        if (shaderType == "VERTEX_SHADER") {
            parseVertexShader(outputDirectory);
        }
        else if (shaderType == "PIXEL_SHADER") {
            parsePixelShader(outputDirectory);
        }
        else {
            std::cout << "error: invalid shader type \"" << shaderType << "\"\n";
            exit(1);
        }
    }

}
