#include <vector>
#include <ramiel/test.h>

using ramiel::test::TestFn;

namespace {

    const std::string divider = "================================================================";

    struct Test {
        std::string name;
        TestFn testFn;
    };

    class TestList {
        std::vector<Test> tests;
        bool curTestPassed = true;
        size_t testsPassed = 0;
    public:
        static TestList& instance() {
            static TestList testList;
            return testList;
        }

        void addTest(const std::string& name, TestFn testFn) {
            tests.push_back({ name, testFn });
        }
        
        void runTests() {
            for (auto& test : tests) {
                std::cout << '\n' << test.name << '\n';

                curTestPassed = true;
                test.testFn();

                std::cout << "\n    "
                          << (curTestPassed ? "PASS" : "FAIL")
                          << "ED!\n";
                
                if (curTestPassed) ++testsPassed;
            }

            std::cout << '\n' << divider << "\n\nsummary:\n\n"
                      << "    tests passed: " << testsPassed << '\n'
                      << "    tests failed: " << tests.size() - testsPassed << '\n';
        }

        void failCurrentTest() {
            curTestPassed = false;
        }
    };

}

namespace ramiel::test {

    void addTest(const std::string& name, TestFn testFn) {
        TestList::instance().addTest(name, testFn);
    }


    void runTests() {
        TestList::instance().runTests();
    }


    void failCurrentTest() {
        TestList::instance().failCurrentTest();
    }

}
