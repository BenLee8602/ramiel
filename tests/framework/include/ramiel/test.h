#pragma once

#include <string>
#include <functional>
#include <iostream>

namespace ramiel::test {

    using TestFn = std::function<void()>;

    void addTest(const std::string& name, TestFn testFn);
    void runTests();
    void failCurrentTest();

}

#define RAMIEL_TEST_ADD(name) \
    void ramielTestFn##name(); \
    namespace { \
        struct RamielTestAdd##name { \
            RamielTestAdd##name() { \
                ramiel::test::addTest(#name, ramielTestFn##name); \
            } \
        } ramielTestAdd##name; \
    } \
    void ramielTestFn##name()

#define RAMIEL_TEST_ASSERT(expression) \
    if (!(expression)) {\
        ramiel::test::failCurrentTest(); \
        std::cout << "\n    assertion failed:\n" \
                  << "        at line " << __LINE__ << " in " << __FILE__ << '\n' \
                  << "        " << #expression << '\n'; \
    }
