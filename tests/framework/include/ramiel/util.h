#pragma once

#include <string>

namespace ramiel::test {

    bool equal(float n0, float n1, float e = 1e-4f);
    bool equal(double n0, double n1, double e = 1e-4);

    std::string readFile(std::string filename);


    class TempFile {
    public:
        TempFile();

        TempFile(const TempFile&) = delete;
        TempFile& operator=(const TempFile&) = delete;

        TempFile(TempFile&&) = default;
        TempFile& operator=(TempFile&&) = default;

        ~TempFile();

        std::string file();

    private:
        std::string filename;
    };

}
