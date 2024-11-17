#include <iostream>
#include "shaderlang.h"


int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "error: incorrect number of arguments ";
        std::cout << "(usage: shaderlang.exe shaderfile output/dir)\n";
        exit(1);
    }
    
    ramiel::shaderlang::compileShader(argv[1], argv[2]);

    return 0;
}
