#include <cassert>

#include <ramiel/data.h>
#include <ramiel/graphics.h>
#include <ramiel/physics.h>
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

    using EngineEntityNode = TreeData<std::unique_ptr<EngineEntity>>;


    std::string pwd() {
        return dir == root ? "/" : dir->getPath().substr(5);
    }


    bool cd(std::string path) {
        Tree::H next = navTree(path);
        if (next) dir = next;
        return next != nullptr;
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


    EngineEntity* get(std::string path) {
        auto node = EngineEntityNode::cast(navTree(path));
        return node ? node->get().get() : nullptr;
    }


    bool mkdir(std::string name) {
        assert(dir);
        if (dir->getKid(name)) return false;
        dir->insert(Tree::make(name));
        return true;
    }


    void add(std::string name, std::unique_ptr<EngineEntity>&& e) {
        assert(dir);
        assert(e);
        e->add();
        dir->insert(EngineEntityNode::make(name, std::move(e)));
    }


    bool rm(std::string name) {
        assert(dir);

        Tree::H erased = dir->erase(name);
        if (!erased) return false;

        Tree::Fn fn = [fn](Tree::H n) {
            auto node = EngineEntityNode::cast(n);
            if (node) node->get()->remove();
            n->forEachKid(fn);
            return true;
        };

        fn(erased);
        return true;
    }

}
