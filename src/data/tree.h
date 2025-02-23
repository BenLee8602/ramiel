#pragma once

#include <string>
#include <memory>
#include <unordered_map>

namespace ramiel {

    class Tree : public std::enable_shared_from_this<Tree> {
    public:
        using H = std::shared_ptr<Tree>;

        static Tree::H make(const std::string& name);

        const std::string& getName() const;
        void setName(const std::string& name);

        Tree::H getKid(const std::string& name) const;
        Tree::H getParent() const;
        Tree::H getRelative(const std::string& path);

        void insert(Tree::H kid);
        Tree::H erase(const std::string& name);

        virtual ~Tree() {}
        
    protected:
        Tree(const std::string& name);

    private:
        std::string name;
        Tree::H parent;
        std::unordered_map<std::string, Tree::H> kids;
    };


    template<class T>
    class TreeData : public Tree {
    public:
        using H = std::shared_ptr<TreeData<T>>;

        template<class... Ts>
        static Tree::H make(const std::string& name, Ts&&... args) {
            return TreeData<T>::H(
                new TreeData<T>(name, std::forward<Ts>(args)...)
            );
        }
        
        static TreeData<T>::H cast(Tree::H t) {
            return std::dynamic_pointer_cast<TreeData<T>>(t);
        }

        T& get() { return data; }
        const T& get() const { return data; }

    protected:
        template<class... Ts>
        TreeData(const std::string& name, Ts&&... args)
            : Tree(name)
            , data(std::forward<Ts>(args)...)
        {}

    private:
        T data;
    };

}
