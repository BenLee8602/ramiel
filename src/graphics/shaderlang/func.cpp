#include <iostream>
#include "func.h"
#include "token.h"

namespace ramiel::shaderlang {

    std::string readShaderFunc() {
        auto& funcHeader = popToken();
        if (funcHeader != "FUNC") {
            std::cout << "error: expected \"FUNC\", got \"" << funcHeader << "\"\n";
            exit(1);
        }

        auto& brace = popToken();
        if (brace != "{") {
            std::cout << "error: expected \"{\", got \"" << brace << "\"\n";
            exit(1);
        }

        std::string shaderFunc;
        std::string prevToken;
        std::string curToken;
        while (tryPopToken(curToken)) {
            shaderFunc += prevToken;
            prevToken = std::move(curToken);
        }

        if (prevToken != "}") {
            std::cout << "error: expected \"}\", got \"" << prevToken << "\"\n";
            exit(1);
        }

        return shaderFunc;
    }

}
