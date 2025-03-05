#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include <ramiel/data.h>
#include <ramiel/graphics.h>
#include <ramiel/physics.h>
using namespace ramiel;

namespace {

    Tree::H root = Tree::make("root");
    Tree::H dir = root;

}

namespace ramiel {

    std::string pwd() {
        return dir == root ? "/" : dir->getPath().substr(5);
    }


    bool cd(std::string path) {
        if (path.empty()) return true;
        Tree::H next = dir;

        if (path[0] == '/') {
            next = root;
            path = path.substr(1);
        }

        next = next->getRelative(path);
        if (!next) return false;

        dir = next;
        return true;
    }


    std::vector<std::string> ls() {
        assert(dir);
        std::vector<std::string> out;
        out.reserve(dir->numKids());

        dir->forEachKid([&out](const Tree::H t) {
            out.emplace_back(t->getName());
            return true;
        });

        return out;
    }


    bool mkdir(std::string name) {
        assert(dir);
        if (dir->getKid(name)) return false;
        dir->insert(Tree::make(name));
        return true;
    }


    bool rm(std::string name) {
        assert(dir);
        return dir->erase(name) == nullptr;
    }

}
