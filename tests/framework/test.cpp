#include <vector>
#include <ramiel/test.h>

using ramiel::test::TestFn;

namespace {

    struct Test {
        std::string name;
        TestFn testFn;
    };
    
    bool curTestPassed;

    class TestList {
        std::vector<Test> tests;
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
            }
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
        curTestPassed = false;
    }

}
