#include <iostream>
#include <fstream>
#include <filesystem>

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


    void writeFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename, std::ios::out | std::ios::trunc);
        if (!file) {
            std::cout << "error: unable to write output\n";
            exit(1);
        }

        file << content;
    }


    std::string absolutePath(const std::string& relativePath) {
        namespace stdfs = std::filesystem;
        static const auto currentPath = stdfs::path(__FILE__).parent_path();
        return stdfs::path(currentPath / relativePath).string();
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

        std::string hTemplate;
        std::string cppTemplate;
        TemplateArgs templateArgs;

        auto& shaderType = popToken();
        auto& shaderName = getToken();

        if (shaderType == "VERTEX_SHADER") {
            hTemplate = readFile(absolutePath("template/vertexShaderHeader"));
            cppTemplate = readFile(absolutePath("template/vertexShaderSource"));
            templateArgs = compileVertexShader();
        }
        else if (shaderType == "PIXEL_SHADER") {
            hTemplate = readFile(absolutePath("template/pixelShaderHeader"));
            cppTemplate = readFile(absolutePath("template/pixelShaderSource"));
            templateArgs = compilePixelShader();
        }
        else {
            std::cout << "error: invalid shader type \"" << shaderType << "\"\n";
            exit(1);
        }

        std::string hOut = fillTemplate(hTemplate, templateArgs);
        std::string cppOut = fillTemplate(cppTemplate, templateArgs);

        std::string hFilename = outputDirectory + '/' + shaderName + ".h";
        std::string cppFilename = outputDirectory + '/' + shaderName + ".cpp";

        writeFile(hFilename, hOut);
        writeFile(cppFilename, cppOut);

    }

}
