#include <fstream>
#include <ramiel/util.h>

namespace ramiel::test {

    bool equal(float n0, float n1, float e) {
        return std::abs(n0 - n1) < e;
    }

    bool equal(double n0, double n1, double e) {
        return std::abs(n0 - n1) < e;
    }


    std::string readFile(std::string filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) return "";
        return std::string((
            std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
    }

}
