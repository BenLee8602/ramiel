#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <sstream>

#include <ramiel/data.h>
#include "command.h"
#include "engine.h"
using namespace ramiel;

namespace {

    using Flag = std::pair<std::string, std::string>;

    struct Command {
        std::vector<std::string> args;
        std::unordered_map<std::string, std::string> flags;
    };


    Flag parseFlag(std::string flag) {
        assert(!flag.empty() && flag[0] == '-');

        size_t equalSign = flag.find('=');

        if (equalSign == std::string::npos)
            return std::make_pair(flag.substr(1), "");

        return std::make_pair(
            flag.substr(1, equalSign - 1),
            flag.substr(equalSign + 1)
        );
    }


    Command parseCommand(std::string command) {
        Command out;
        std::istringstream ss(command);
        std::string token;
        while (std::getline(ss, token, ' ')) {
            if (token.empty()) continue;
            else if (token[0] == '-')
                out.flags.insert(parseFlag(token));
            else
                out.args.emplace_back(std::move(token));
        }
        return out;
    }


    void nav(Command cmd) {
        if (cmd.args.size() != 2) return;
        ramiel::nav(cmd.args[1]);
    }

    void make_dir(Command cmd) {
        if (cmd.args.size() != 3) return;
        ramiel::make_dir(cmd.args[2]);
    }

    void get_path(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << ramiel::get_path() << '\n';
    }

    void get_name(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << ramiel::get_name() << '\n';
    }

    void get_kids(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::vector<std::string> kids = ramiel::get_kids();
        std::cout << "[\n";
        for (auto& k : kids) {
            std::cout << "    " << k << '\n';
        }
        std::cout << "]\n";
    }

    void set_name(Command cmd) {
        if (cmd.args.size() != 3) return;
        ramiel::set_name(cmd.args[2]);
    }

    void del(Command cmd) {
        if (cmd.args.size() != 2) return;
        ramiel::del(cmd.args[1]);
    }


    using CommandNode = TreeData<std::function<void(Command)>>;

    Tree::H getCommandTree() {
        static Tree::H cmdTree = nullptr;
        if (cmdTree) return cmdTree;

        cmdTree = Tree::make("commands");

        cmdTree->insert(CommandNode::make("nav", nav));

        Tree::H cmdTreeMake = Tree::make("make");
        cmdTreeMake->insert(CommandNode::make("dir", make_dir));
        cmdTree->insert(cmdTreeMake);

        Tree::H cmdTreeGet = Tree::make("get");
        cmdTreeGet->insert(CommandNode::make("path", get_path));
        cmdTreeGet->insert(CommandNode::make("name", get_name));
        cmdTreeGet->insert(CommandNode::make("kids", get_kids));
        cmdTree->insert(cmdTreeGet);

        Tree::H cmdTreeSet = Tree::make("set");
        cmdTreeSet->insert(CommandNode::make("name", set_name));
        cmdTree->insert(cmdTreeSet);

        cmdTree->insert(CommandNode::make("del", del));

        return cmdTree;
    }

}

namespace ramiel {

    void runCommand(std::string command) {
        Command cmd = parseCommand(command);
        Tree::H cmdNode = getCommandTree();
        
        for (auto& arg : cmd.args) {
            cmdNode = cmdNode->getKid(arg);
            if (!cmdNode) break;
            
            CommandNode::H cmdFn = CommandNode::cast(cmdNode);
            if (!cmdFn) continue;

            assert(cmdFn->numKids() == 0);
            cmdFn->get()(cmd);
            break;
        }
    }

}
