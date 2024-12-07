#pragma once

#include <string>
#include <unordered_map>

namespace ramiel::shaderlang {

    using TemplateArgs = std::unordered_map<std::string, std::string>;

    std::string fillTemplate(const std::string& temp, const TemplateArgs& args);

}
