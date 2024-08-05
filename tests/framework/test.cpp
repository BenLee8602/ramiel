#include <unordered_map>
#include <vector>
#include <ramiel/test.h>

using ramiel::test::TestFn;

namespace {

    const std::string divider = std::string(64, '=');

    std::string resStr(bool res) {
        return std::string(res ? "PASS" : "FAIL") + "ED";
    }

    struct Test {
        std::string name;
        TestFn testFn;
    };

    class TestList {
        using TestGroup = std::vector<Test>;
        using GroupList = std::unordered_map<std::string, TestGroup>;

        GroupList testGroups;
        size_t numTests = 0;

        bool curGroupPassed = true;
        bool curTestPassed = true;

    public:
        static TestList& instance() {
            static TestList testList;
            return testList;
        }

        void addTest(const std::string& file, const std::string& name, TestFn testFn) {
            testGroups[file].push_back({ name, testFn });
            ++numTests;
        }

        void failCurrentTest() {
            curTestPassed = false;
            curGroupPassed = false;
        }
        
        void runTests() {
            size_t groupsPassed = 0;
            size_t testsPassed = 0;

            for (auto& group : testGroups) {
                std::cout << group.first << "\n\n";
                curGroupPassed = true;

                for (auto& test : group.second) {
                    std::cout << "    " << test.name << '\n';
                    curTestPassed = true;
                    test.testFn();
                    if (curTestPassed) ++testsPassed;
                    std::cout << "        " << resStr(curTestPassed) << "\n\n";
                }

                if (curGroupPassed) ++groupsPassed;
                std::cout << '\n';
            }

            std::cout << divider << "\n\nsummary:\n\n"
                      << "    total groups: " << testGroups.size() << '\n'
                      << "    total tests: " << numTests << "\n\n"
                      << "    groups passed: " << groupsPassed << '\n'
                      << "    tests passed: " << testsPassed << "\n\n"
                      << "    groups failed: " << testGroups.size() - groupsPassed << '\n'
                      << "    tests failed: " << numTests - testsPassed << '\n';
        }
    };

}

namespace ramiel::test {

    void addTest(const std::string& file, const std::string& name, TestFn testFn) {
        TestList::instance().addTest(file, name, testFn);
    }

    void failCurrentTest() {
        TestList::instance().failCurrentTest();
    }

    void runTests() {
        TestList::instance().runTests();
    }

}
