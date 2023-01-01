#include <iostream>
#include "src/rotation.h"
using namespace ramiel;

int main() {
    Rotation rot;
    rot.set({ 5.9f, -9.0f, -2.1f });
    std::cout << rot.sin() << rot.cos() << '\n';
    std::cin.get();
}
