#include <cassert>
#include <vector>
#include <sstream>
#include <algorithm>

#include "tree.h"
using namespace ramiel;

namespace {

    bool validName(const std::string& name) {
        return name != "parent" && name.find('.') == std::string::npos;
    }


    std::vector<std::string> split(std::string str, char delim) {
        std::istringstream s(str);
        std::vector<std::string> out;
        out.reserve(std::count(str.begin(), str.end(), delim) + 1);
        
        while (std::getline(s, str, delim)) {
            out.emplace_back(std::move(str));
        }

        return out;
    }


    bool isAncestorOf(Tree::H t1, Tree::H t2) {
        while (t2) {
            t2 = t2->getParent();
            if (t2 == t1) return true;
        }
        return false;
    }

}

namespace ramiel {

    Tree::H Tree::make(const std::string& name) {
        return Tree::H(new Tree(name));
    }

    Tree::Tree(const std::string& name)
        : name(name)
    {
        assert(validName(name));
    }


    const std::string& Tree::getName() const {
        return name;
    }

    void Tree::setName(const std::string& name) {
        assert(validName(name));
        if (parent) {
            assert(!parent->getKid(name));
            Tree::H cur = parent->getKid(this->name);
            assert(cur == shared_from_this());
            parent->kids.erase(this->name);
            parent->kids.insert(std::make_pair(name, cur));
        }
        this->name = name;
    }


    Tree::H Tree::getKid(const std::string& name) const {
        auto kid = kids.find(name);
        return kid == kids.end() ? nullptr : kid->second;
    }

    Tree::H Tree::getParent() const {
        return parent;
    }

    Tree::H Tree::getRelative(const std::string& path) {
        Tree::H cur = shared_from_this();
        for (auto& s : split(path, '.')) {
            if (!cur) break;
            cur = s == "parent" ? cur->getParent() : cur->getKid(s);
        }
        return cur;
    }
    

    void Tree::insert(Tree::H kid) {
        assert(kid);
        assert(kid != shared_from_this());
        assert(!isAncestorOf(kid, shared_from_this()));
        assert(!getKid(kid->name));
        kids.insert(std::make_pair(kid->name, kid));
        if (kid->parent) kid->parent->kids.erase(kid->name);
        kid->parent = shared_from_this();
    }

    Tree::H Tree::erase(const std::string& name) {
        Tree::H kid = getKid(name);
        if (!kid) return nullptr;
        assert(kid->name == name);
        assert(kid->parent == shared_from_this());
        kid->parent = nullptr;
        kids.erase(name);
        return kid;
    }

}
