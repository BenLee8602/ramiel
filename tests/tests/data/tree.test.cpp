#include <ramiel/test.h>
#include <ramiel/data.h>
using namespace ramiel;

//      1
//     / \ 
//    2   3
//   / \   \ 
//  4   5   6

RAMIEL_TEST_ADD(Tree) {
    Tree::H one = Tree::make("one");
    Tree::H two = Tree::make("two");
    Tree::H three = Tree::make("three");
    Tree::H four = Tree::make("four");
    Tree::H five = Tree::make("five");
    Tree::H six = TreeData<int>::make("six", 7);

    one->insert(two);
    one->insert(three);
    two->insert(four);
    two->insert(five);
    three->insert(six);

    two->setName("second");
    RAMIEL_TEST_ASSERT(two->getName() == "second");
    RAMIEL_TEST_ASSERT(one->getKid("second") == two);

    RAMIEL_TEST_ASSERT(three->getKid("six") == six);

    RAMIEL_TEST_ASSERT(four->getParent() == two);
    RAMIEL_TEST_ASSERT(one->getParent() == nullptr);

    RAMIEL_TEST_ASSERT(five->getRelative("parent.parent.three") == three);
    RAMIEL_TEST_ASSERT(six->getRelative("parent.null") == nullptr);
    RAMIEL_TEST_ASSERT(two->getRelative("parent.parent") == nullptr);

    RAMIEL_TEST_ASSERT(one->erase("second") == two);
    RAMIEL_TEST_ASSERT(one->getKid("second") == nullptr);
    RAMIEL_TEST_ASSERT(two->getParent() == nullptr);

    TreeData<int>::H three_ = TreeData<int>::cast(three);
    RAMIEL_TEST_ASSERT(!three_);

    TreeData<int>::H six_ = TreeData<int>::cast(six);
    RAMIEL_TEST_ASSERT(six_);
    RAMIEL_TEST_ASSERT(six_->get() == 7);
}
