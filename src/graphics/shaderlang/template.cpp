#include <sstream>
#include "template.h"

namespace {

    std::string parseTemplateArg(std::istream& stream) {
        std::ostringstream out;

        char c = 0;
        if (!stream.get(c) || c != '{') return "";

        while (stream.get(c)) {
            if (c == '}') return out.str();
            out << c;
        }
        return "";
    }

}

namespace ramiel::shaderlang {

    std::string fillTemplate(const std::string& temp, const TemplateArgs& args) {
        std::istringstream stream(temp);
        std::ostringstream out;

        char c;
        while (stream.get(c)) {
            if (c != '$') {
                out << c;
                continue;
            }

            std::string arg = parseTemplateArg(stream);
            if (arg.empty()) return "";

            auto it = args.find(arg);
            if (it == args.end()) return "";

            out << it->second;
        }

        return out.str();
    }

}
