#include <iostream>
#include "func.h"
#include "token.h"

namespace {

    bool isPunctuation(const std::string& token) {
        return token.length() == 1 && !std::isalnum(token[0]) && token[0] != '_';
    }

}

namespace ramiel::shaderlang {

    std::string readShaderFunc(const std::string& header) {
        auto& headerToken = popToken();
        if (headerToken != header) {
            std::cout << "error: expected \"" << header << "\", got \"" << headerToken << "\"\n";
            exit(1);
        }

        auto& brace = popToken();
        if (brace != "{") {
            std::cout << "error: expected \"{\", got \"" << brace << "\"\n";
            exit(1);
        }

        std::string shaderFunc = "{";
        size_t braceDepth = 1;
        
        while (braceDepth) {
            auto& token = popToken();
            if (token == "{") braceDepth++;
            else if (token == "}") braceDepth--;
            shaderFunc += token;

            // this is a freaky workaround until we stop relying on c++
            if (!isPunctuation(token)) shaderFunc += ' ';
        }

        return shaderFunc;
    }

}
