#include <fstream>
#include <filesystem>

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


    TempFile::TempFile() {
        static size_t count = 0;
        filename = std::filesystem::temp_directory_path().string();
        filename += "ramielTestTempFile" + std::to_string(count++);
    }

    TempFile::~TempFile() {
        std::remove(filename.c_str());
    }

    std::string TempFile::file() {
        return filename;
    }

}
