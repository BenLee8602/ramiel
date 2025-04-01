#include <cassert>

#include <ramiel/data.h>
#include "engine.h"
using namespace ramiel;

namespace {

    Tree::H root = Tree::make("root");
    Tree::H dir = root;


    Tree::H navTree(std::string path) {
        if (path.empty()) return dir;
        Tree::H next = dir;

        if (path[0] == '/') {
            next = root;
            path = path.substr(1);
        }

        next = next->getRelative(path);
        return next;
    }

}

namespace ramiel {

    void nav(std::string path) {
        Tree::H next = navTree(path);
        if (next) dir = next;
    }


    void make_dir(std::string name) {
        assert(dir);
        if (!Tree::validName(name)) return;
        if (dir->getKid(name)) return;
        dir->insert(Tree::make(name));
    }


    std::string get_path() {
        assert(dir && root);
        return dir == root ? "/" : dir->getPath().substr(5);
    }


    std::string get_name() {
        assert(dir);
        return dir->getName();
    }


    std::vector<std::string> get_kids() {
        assert(dir);
        std::vector<std::string> kids;
        kids.reserve(dir->numKids());
        dir->forEachKid([&kids](Tree::H kid) {
            kids.push_back(kid->getName());
            return true;
        });
        return kids;
    }


    void set_name(std::string name) {
        assert(dir);
        if (!Tree::validName(name)) return;
        if (dir->getParent() && dir->getParent()->getKid(name)) return;
        dir->setName(name);
    }


    void del(std::string kid) {
        assert(dir);
        dir->erase(kid);
    }

}
