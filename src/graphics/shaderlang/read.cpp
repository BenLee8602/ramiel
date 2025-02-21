#include <iostream>
#include "read.h"
#include "token.h"

namespace {

    constexpr size_t maxAttrs = 16;

    bool isValidAttrName(const std::string& token) {
        return std::isalpha(token[0]) || token[0] == '_';
    }

    bool isPunctuation(const std::string& token) {
        return token.length() == 1 && !std::isalnum(token[0]) && token[0] != '_';
    }

}

namespace ramiel::shaderlang {

    Attr::Attr(
        const std::string& type,
        const std::string& name
    ) :
        type(type),
        name(name)
    {}

    Attr::Attr(
        std::string&& type,
        std::string&& name
    ) :
        type(std::move(type)),
        name(std::move(name))
    {}


    std::vector<Attr> readAttrList(const std::string& header) {
        auto& headerToken = popToken();
        if (headerToken != header) {
            std::cout << "error: expected \"" << header << "\", got \"" << headerToken << "\"\n";
            exit(1);
        }

        auto& braceToken = popToken();
        if (braceToken != "{") {
            std::cout << "error: expected \"{\", got \"" << braceToken << "\"\n";
            exit(1);
        }

        std::vector<Attr> attrs;
        attrs.reserve(maxAttrs);

        for (size_t i = 0; i < maxAttrs; i++) {
            std::string attrType = popToken();
            if (attrType == "}") {
                return attrs;
            }

            std::string attrName = popToken();
            if (!isValidAttrName(attrName)) {
                std::cout << "error: invalid attribute name \"" << attrName << "\"\n";
                exit(1);
            }

            attrs.emplace_back(std::move(attrType), std::move(attrName));
        }

        std::cout << "error: exceeded maximum allowed attributes (" << maxAttrs << ")\n";
        exit(1);

        return attrs;
    }


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
