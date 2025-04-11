#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <charconv>
#include <algorithm>

#include <ramiel/data.h>
#include "command.h"
#include "graphics.h"
#include "task.h"
#include "window.h"
#include "entity.h"
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

    bool insertTree(Tree::H t) {
        assert(t);
        if (!Tree::validName(t->getName())) return false;
        if (dir->getKid(t->getName())) return false;
        dir->insert(t);
        return true;
    }


    template<typename T>
    bool fromString(const std::string& src, T& des) {
        auto first = src.data();
        auto last = src.data() + src.size();
        auto res = std::from_chars(first, last, des);
        return res.ec == std::errc() && res.ptr == last;
    }

    template<typename T, size_t N>
    bool fromString(const std::string& src, Vec<T, N>& des) {
        if (std::count(src.begin(), src.end(), ',') != N - 1) return false;
        std::istringstream srcstr(src);
        for (size_t i = 0; i < N; i++) {
            std::string elem;
            if (!std::getline(srcstr, elem, ',')) return false;
            if (!fromString(elem, des[i])) return false;
        }
        return true;
    }


    using Flag = std::pair<std::string, std::string>;

    struct Command {
        std::vector<std::string> args;
        std::unordered_map<std::string, std::string> flags;

        std::string getFlag(
            const std::string& name,
            const char* defaultValue = nullptr
        ) {
            auto flag = flags.find(name);
            if (flag != flags.end() && !flag->second.empty()) {
                return flag->second;
            }

            bool manual = flags.find("manual") != flags.end();
            if (!manual && defaultValue) return defaultValue;

            std::string value;
            std::cout << "enter property " << name << ": ";
            std::getline(std::cin, value);
            return value;
        }

        bool hasFlag(const std::string& name) {
            return flags.find(name) != flags.end();
        }
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
        Tree::H next = navTree(cmd.args[1]);
        if (!next) return;
        dir = next;
    }

    void make_dir(Command cmd) {
        assert(dir);
        if (cmd.args.size() != 3) return;
        insertTree(Tree::make(cmd.args[2]));
    }

    void make_mesh(Command cmd) {
        if (cmd.args.size() != 3) return;

        std::string filename = cmd.getFlag("filename");
        
        EngineMesh::H mesh = EngineEntity::make<EngineMesh>(
            cmd.args[2], filename
        );
        if (mesh->get()->getTriangleCount() == 0) return;

        insertTree(mesh);
    }

    void make_texture(Command cmd) {
        if (cmd.args.size() != 3) return;

        std::string filename = cmd.getFlag("filename");

        EngineTexture::H texture = EngineEntity::make<EngineTexture>(
            cmd.args[2], filename, rgb1
        );
        if (texture->get()->getSize() == Vec2u{}) return;

        insertTree(texture);
    }

    void make_entity(Command cmd) {
        if (cmd.args.size() != 3) return;

        std::string meshPath = cmd.getFlag("mesh");
        EngineMesh::H mesh = EngineEntity::cast<EngineMesh>(navTree(meshPath));
        if (!mesh) return;

        std::string vsPosStr = cmd.getFlag("vs.pos", "0,0,0");
        Vec3f vsPos;
        if (!fromString(vsPosStr, vsPos)) return;

        std::string vsRotStr = cmd.getFlag("vs.rot", "0,0,0");
        Vec3f vsRot;
        if (!fromString(vsRotStr, vsRot)) return;

        std::string vsScaleStr = cmd.getFlag("vs.scale", "1,1,1");
        Vec3f vsScale;
        if (!fromString(vsScaleStr, vsScale)) return;

        std::string psSpecExponentStr = cmd.getFlag("ps.specexponent", "8");
        float psSpecExponent;
        if (!fromString(psSpecExponentStr, psSpecExponent)) return;

        std::string psSpecIntensityStr = cmd.getFlag("ps.specintensity", "1");
        float psSpecIntensity;
        if (!fromString(psSpecIntensityStr, psSpecIntensity)) return;

        std::unique_ptr<VertexShaderBase> vs;
        std::unique_ptr<PixelShaderBase> ps;

        if (cmd.hasFlag("texture")) {
            std::string psTexturePath = cmd.getFlag("texture");
            EngineTexture::H texture = EngineEntity::cast<EngineTexture>(
                navTree(psTexturePath));
            vs = std::make_unique<VertexShaderTextured>(
                matmat(matmat(scale(vsScale), rotate(vsRot)), translate(vsPos)));
            ps = std::make_unique<PixelShaderTextured>(
                texture->get(), psSpecExponent, psSpecIntensity, Vec3f{});
        } else {
            std::string psColorStr = cmd.getFlag("color", "255,255,255");
            Vec3f psColor;
            if (!fromString(psColorStr, psColor)) return;
            vs = std::make_unique<VertexShader>(
                matmat(matmat(scale(vsScale), rotate(vsRot)), translate(vsPos)));
            ps = std::make_unique<PixelShader>(
                psColor / 255.0f, psSpecExponent, psSpecIntensity, Vec3f{});
        }

        EngineGraphicsEntity::H entity = EngineEntity::make<EngineGraphicsEntity>(
            cmd.args[2], mesh->get(), std::move(vs), std::move(ps)
        );
        if (!entity) return;

        insertTree(entity);
    }

    void make_dirlight(Command cmd) {
        if (cmd.args.size() != 3) return;

        std::string colorStr = cmd.getFlag("color", "255,255,255");
        Vec3f color;
        if (!fromString(colorStr, color)) return;

        std::string intensityStr = cmd.getFlag("intensity", "1");
        float intensity;
        if (!fromString(intensityStr, intensity)) return;

        std::string dirString = cmd.getFlag("dir");
        Vec3f dir;
        if (!fromString(dirString, dir)) return;

        EngineDirectionalLight::H light = EngineEntity::make<EngineDirectionalLight>(
            cmd.args[2], color, intensity, dir
        );

        insertTree(light);
    }

    void make_pointlight(Command cmd) {
        if (cmd.args.size() != 3) return;

        std::string colorStr = cmd.getFlag("color", "255,255,255");
        Vec3f color;
        if (!fromString(colorStr, color)) return;

        std::string intensityStr = cmd.getFlag("intensity", "1");
        float intensity;
        if (!fromString(intensityStr, intensity)) return;

        std::string posStr = cmd.getFlag("pos");
        Vec3f pos;
        if (!fromString(posStr, pos)) return;

        std::string falloffStr = cmd.getFlag("falloff", "1");
        float falloff;
        if (!fromString(falloffStr, falloff)) return;

        EnginePointLight::H light = EngineEntity::make<EnginePointLight>(
            cmd.args[2], color, intensity, pos, falloff
        );

        insertTree(light);
    }

    void make_spotlight(Command cmd) {
        if (cmd.args.size() != 3) return;

        std::string colorStr = cmd.getFlag("color", "255,255,255");
        Vec3f color;
        if (!fromString(colorStr, color)) return;

        std::string intensityStr = cmd.getFlag("intensity", "1");
        float intensity;
        if (!fromString(intensityStr, intensity)) return;

        std::string posStr = cmd.getFlag("pos");
        Vec3f pos;
        if (!fromString(posStr, pos)) return;

        std::string dirString = cmd.getFlag("dir");
        Vec3f dir;
        if (!fromString(dirString, dir)) return;

        std::string falloffStr = cmd.getFlag("falloff", "1");
        float falloff;
        if (!fromString(falloffStr, falloff)) return;

        std::string widthStr = cmd.getFlag("width", "0.785398");
        float width;
        if (!fromString(widthStr, width)) return;

        std::string falloffExpStr = cmd.getFlag("falloffexp", "1");
        float falloffExp;
        if (!fromString(falloffExpStr, falloffExp)) return;

        EngineSpotLight::H light = EngineEntity::make<EngineSpotLight>(
            cmd.args[2], color, intensity, pos, dir, falloff, width, falloffExp
        );

        insertTree(light);
    }


    void get_path(Command cmd) {
        assert(dir && root);
        if (cmd.args.size() != 2) return;
        std::cout << getPath() << '\n';
    }

    void get_name(Command cmd) {
        assert(dir);
        if (cmd.args.size() != 2) return;
        std::cout << dir->getName() << '\n';
    }

    void get_kids(Command cmd) {
        assert(dir);
        if (cmd.args.size() != 2) return;
        std::cout << "[\n";
        dir->forEachKid([](Tree::H kid) {
            std::cout << "    " << kid->getName() << '\n';
            return true;
        });
        std::cout << "]\n";
    }

    void get_cameraRes(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getRes() << '\n';
    }

    void get_cameraAspectRatio(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getAspectRatio() << '\n';
    }

    void get_cameraPos(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getPos() << '\n';
    }

    void get_cameraRot(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getRot() << '\n';
    }

    void get_cameraFov(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getFov() << '\n';
    }

    void get_cameraFocalLength(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getFocalLen() << '\n';
    }

    void get_ambientLight(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getAmbientLight() << '\n';
    }

    void get_backgroundColor(Command cmd) {
        if (cmd.args.size() != 2) return;
        std::cout << getBackgroundColor() << '\n';
    }

    void set_name(Command cmd) {
        assert(dir && root);
        if (cmd.args.size() != 3) return;
        if (dir == root) return;
        if (!Tree::validName(cmd.args[2])) return;
        if (dir->getParent()->getKid(cmd.args[2])) return;
        dir->setName(cmd.args[2]);
    }

    void set_cameraRes(Command cmd) {
        if (cmd.args.size() != 4) return;
        Vec2u res;
        if (!fromString(cmd.args[2], res[X])) return;
        if (!fromString(cmd.args[3], res[Y])) return;
        if (res[X] == 0 || res[Y] == 0) return;
        addTask([res]() { setWindowSize(res); });
    }

    void set_cameraPos(Command cmd) {
        if (cmd.args.size() != 5) return;
        Vec3f pos;
        if (!fromString(cmd.args[2], pos[X])) return;
        if (!fromString(cmd.args[3], pos[Y])) return;
        if (!fromString(cmd.args[4], pos[Z])) return;
        addTask([pos]() { setPos(pos); });
    }

    void set_cameraRot(Command cmd) {
        if (cmd.args.size() != 5) return;
        Vec3f rot;
        if (!fromString(cmd.args[2], rot[X])) return;
        if (!fromString(cmd.args[3], rot[Y])) return;
        if (!fromString(cmd.args[4], rot[Z])) return;
        addTask([rot]() { setRot(rot); });
    }

    void set_cameraFov(Command cmd) {
        if (cmd.args.size() != 3) return;
        float fov;
        if (!fromString(cmd.args[2], fov)) return;
        if (fov < 1e-6f || fov > 180.0f) return;
        addTask([fov]() { setFov(fov); });
    }

    void set_cameraFocalLength(Command cmd) {
        if (cmd.args.size() != 3) return;
        float focalLength;
        if (!fromString(cmd.args[2], focalLength)) return;
        if (focalLength < 1e-6f) return;
        addTask([focalLength]() { setFocalLen(focalLength); });
    }

    void set_ambientLight(Command cmd) {
        if (cmd.args.size() != 5) return;
        Vec3f ambientLight;
        if (!fromString(cmd.args[2], ambientLight[R])) return;
        if (!fromString(cmd.args[3], ambientLight[G])) return;
        if (!fromString(cmd.args[4], ambientLight[B])) return;
        if (ambientLight < 0.0f) return;
        addTask([ambientLight]() { setAmbientLight(ambientLight); });
    }

    void set_backgroundColor(Command cmd) {
        if (cmd.args.size() != 5) return;
        Vec3f backgroundColor;
        if (!fromString(cmd.args[2], backgroundColor[R])) return;
        if (!fromString(cmd.args[3], backgroundColor[G])) return;
        if (!fromString(cmd.args[4], backgroundColor[B])) return;
        addTask([backgroundColor]() { setBackgroundColor(backgroundColor); });
    }

    void del(Command cmd) {
        assert(dir);
        if (cmd.args.size() != 2) return;
        Tree::H tree = dir->erase(cmd.args[1]);
        addTask([tree]() {});
    }


    using CommandNode = TreeData<std::function<void(Command)>>;

    Tree::H getCommandTree() {
        static Tree::H cmdTree = nullptr;
        if (cmdTree) return cmdTree;

        cmdTree = Tree::make("commands");

        cmdTree->insert(CommandNode::make("nav", nav));

        Tree::H cmdTreeMake = Tree::make("make");
        cmdTreeMake->insert(CommandNode::make("dir", make_dir));
        cmdTreeMake->insert(CommandNode::make("mesh", make_mesh));
        cmdTreeMake->insert(CommandNode::make("texture", make_texture));
        cmdTreeMake->insert(CommandNode::make("entity", make_entity));
        cmdTreeMake->insert(CommandNode::make("dirlight", make_dirlight));
        cmdTreeMake->insert(CommandNode::make("pointlight", make_pointlight));
        cmdTreeMake->insert(CommandNode::make("spotlight", make_spotlight));
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


    std::string getPath() {
        return dir == root ? "/" : dir->getPath().substr(5);
    }

}
