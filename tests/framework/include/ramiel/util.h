#pragma once

#include <string>

namespace ramiel::test {

    bool equal(float n0, float n1, float e = 1e-4f);
    bool equal(double n0, double n1, double e = 1e-4);

    std::string readFile(std::string filename);

}
