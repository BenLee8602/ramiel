#pragma once

#include <string>
#include <vector>

namespace ramiel::shaderlang {

    struct Attr {
        std::string type;
        std::string name;

        Attr() = default;
        Attr(const std::string& type, const std::string& name);
        Attr(std::string&& type, std::string&& name);
    };

    std::vector<Attr> readAttrList(const std::string& header);

}
