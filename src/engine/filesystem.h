#pragma once

#include <vector>
#include <string>

namespace ramiel {

    std::string pwd();
    bool cd(std::string path);
    std::vector<std::string> ls();
    bool mkdir(std::string name);
    bool rm(std::string name);

}
