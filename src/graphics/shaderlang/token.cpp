#include <iostream>
#include <vector>
#include "token.h"

namespace {

    std::vector<std::string> tokens;
    size_t i;

}

namespace ramiel::shaderlang {

    void tokenize(const std::string& in) {
        tokens = std::vector<std::string>();
        i = 0;

        std::string token;

        for (auto c : in) {
            if (std::isalnum(c) || c == '_') {
                token += c;
                continue;
            }

            if (!token.empty()) {
                tokens.emplace_back(std::move(token));
            }

            if (!std::isspace(c)) {
                tokens.emplace_back(1, c);
            }
        }

        if (!token.empty()) {
            tokens.emplace_back(std::move(token));
        }
    }


    const std::string& getToken() {
        if (i < tokens.size()) {
            return tokens[i];
        }

        std::cout << "error: expected token, got eof\n";
        exit(1);
    }


    const std::string& popToken() {
        if (i < tokens.size()) {
            return tokens[i++];
        }

        std::cout << "error: expected token, got eof\n";
        exit(1);
    }


    bool tryPopToken(std::string& out) {
        if (i >= tokens.size()) {
            return false;
        }

        out = tokens[i++];
        return true;
    }

}
