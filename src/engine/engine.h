#pragma once

#include <string>
#include <vector>

namespace ramiel {

    void nav(std::string path);

    void make_dir(std::string name);

    std::string get_path();
    std::string get_name();
    std::vector<std::string> get_kids();

    void set_name(std::string name);

    void del(std::string kid);

}
