#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <charconv>

#include <ramiel/data.h>
#include "command.h"
#include "engine.h"
#include "graphics.h"
using namespace ramiel;

namespace {

    template<typename T>
    bool fromString(std::string src, T& des) {
        auto first = src.data();
        auto last = src.data() + src.size();
        auto res = std::from_chars(first, last, des);
        return res.ec == std::errc() && res.ptr == last;
    }


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

    void get_cameraRes(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getCameraRes() << '\n';
    }

    void get_cameraAspectRatio(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getCameraAspectRatio() << '\n';
    }

    void get_cameraPos(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getCameraPos() << '\n';
    }

    void get_cameraRot(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getCameraRot() << '\n';
    }

    void get_cameraFov(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getCameraFov() << '\n';
    }

    void get_cameraFocalLength(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getCameraFocalLength() << '\n';
    }

    void get_ambientLight(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getAmbientLightColor() << '\n';
    }

    void get_backgroundColor(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getBackgroundColor() << '\n';
    }

    void set_name(Command cmd) {
        if (cmd.args.size() != 3) return;
        ramiel::set_name(cmd.args[2]);
    }

    void set_cameraRes(Command cmd) {
        if (cmd.args.size() != 4) return;
        Vec2u res;
        if (!fromString(cmd.args[2], res[X])) return;
        if (!fromString(cmd.args[3], res[Y])) return;
        // todo: resize window
    }

    void set_cameraPos(Command cmd) {
        if (cmd.args.size() != 5) return;
        Vec3f pos;
        if (!fromString(cmd.args[2], pos[X])) return;
        if (!fromString(cmd.args[3], pos[Y])) return;
        if (!fromString(cmd.args[4], pos[Z])) return;
        setCameraPos(pos);
    }

    void set_cameraRot(Command cmd) {
        if (cmd.args.size() != 5) return;
        Vec3f rot;
        if (!fromString(cmd.args[2], rot[X])) return;
        if (!fromString(cmd.args[3], rot[Y])) return;
        if (!fromString(cmd.args[4], rot[Z])) return;
        setCameraRot(rot);
    }

    void set_cameraFov(Command cmd) {
        if (cmd.args.size() != 3) return;
        float fov;
        if (!fromString(cmd.args[2], fov)) return;
        setCameraFov(fov);
    }

    void set_cameraFocalLength(Command cmd) {
        if (cmd.args.size() != 3) return;
        float focalLength;
        if (!fromString(cmd.args[2], focalLength)) return;
        setCameraFocalLength(focalLength);
    }

    void set_ambientLight(Command cmd) {
        if (cmd.args.size() != 5) return;
        Vec3f ambientLight;
        if (!fromString(cmd.args[2], ambientLight[R])) return;
        if (!fromString(cmd.args[3], ambientLight[G])) return;
        if (!fromString(cmd.args[4], ambientLight[B])) return;
        setAmbientLightColor(ambientLight);
    }

    void set_backgroundColor(Command cmd) {
        if (cmd.args.size() != 5) return;
        Vec3f backgroundColor;
        if (!fromString(cmd.args[2], backgroundColor[R])) return;
        if (!fromString(cmd.args[3], backgroundColor[G])) return;
        if (!fromString(cmd.args[4], backgroundColor[B])) return;
        setBackgroundColor(backgroundColor);
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
        cmdTreeGet->insert(CommandNode::make("cameraRes", get_cameraRes));
        cmdTreeGet->insert(CommandNode::make("cameraAspectRatio", get_cameraAspectRatio));
        cmdTreeGet->insert(CommandNode::make("cameraPos", get_cameraPos));
        cmdTreeGet->insert(CommandNode::make("cameraRot", get_cameraRot));
        cmdTreeGet->insert(CommandNode::make("cameraFov", get_cameraFov));
        cmdTreeGet->insert(CommandNode::make("cameraFocalLength", get_cameraFocalLength));
        cmdTreeGet->insert(CommandNode::make("ambientLight", get_ambientLight));
        cmdTreeGet->insert(CommandNode::make("backgroundColor", get_backgroundColor));
        cmdTree->insert(cmdTreeGet);

        Tree::H cmdTreeSet = Tree::make("set");
        cmdTreeSet->insert(CommandNode::make("name", set_name));
        cmdTreeSet->insert(CommandNode::make("cameraRes", set_cameraRes));
        cmdTreeSet->insert(CommandNode::make("cameraPos", set_cameraPos));
        cmdTreeSet->insert(CommandNode::make("cameraRot", set_cameraRot));
        cmdTreeSet->insert(CommandNode::make("cameraFov", set_cameraFov));
        cmdTreeSet->insert(CommandNode::make("cameraFocalLength", set_cameraFocalLength));
        cmdTreeSet->insert(CommandNode::make("ambientLight", set_ambientLight));
        cmdTreeSet->insert(CommandNode::make("backgroundColor", set_backgroundColor));
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
