#pragma once

#include <string>

namespace ramiel::shaderlang {

    void compileShader(
        const std::string& inputFilename,
        const std::string& outputDirectory
    );

}
