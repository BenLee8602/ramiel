#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>

#include <ramiel/data.h>
#include "command.h"
#include "graphics.h"
#include "physics.h"
#include "task.h"
#include "window.h"
#include "entity.h"
#include "serialize.h"
using namespace ramiel;

namespace {

    Tree::H root = Tree::make("root");
    Tree::H dir = root;

    Tree::H rootSnap;
    Tree::H dirSnap;

    bool insertTree(Tree::H t) {
        assert(t);
        if (!Tree::validName(t->getName())) return false;
        if (dir->getKid(t->getName())) return false;
        dir->insert(t);
        return true;
    }


    using Flag = std::pair<std::string, std::string>;

    struct Command {
        std::vector<std::string> args;
        std::unordered_map<std::string, std::string> flags;

        std::string getFlag(
            const std::string& name,
            const char* defaultValue = nullptr
        ) const {
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

        bool hasFlag(const std::string& name) const {
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


    bool parseParticle(const Command& cmd, Particle& p) {
        if (!fromString(cmd.getFlag("pos", "0,0,0"), p.pos)) return false;
        if (!fromString(cmd.getFlag("vel", "0,0,0"), p.vel)) return false;
        
        float mass;
        if (!fromString(cmd.getFlag("mass", "0"), mass)) return false;
        if (mass > 0.0f) return false;
        p.wass = mass ? 1.0f / mass : 0.0f;

        return true;
    }

    bool parseRigidBody(const Command& cmd, RigidBody& rb) {
        if (!parseParticle(cmd, rb)) return false;

        if (!fromString(cmd.getFlag("rot", "0,0,0,1"), rb.rot)) return false;
        if (!fromString(cmd.getFlag("rotaxis", "0,0,0"), rb.rotAxis)) return false;
        
        Vec3f moi;
        if (!fromString(cmd.getFlag("moi", "1,1,1"), moi)) return false;
        if (!(moi >= 0.0f)) return false;
        rb.woi = { 1.0f / moi[X], 1.0f / moi[Y], 1.0f / moi[Z] };

        return true;
    }


    EngineParticle::H makeParticle(Command cmd) {
        if (cmd.args.size() != 3) return nullptr;

        Particle e;
        if (!parseParticle(cmd, e)) return nullptr;

        return EngineEntity::make<EngineParticle>(cmd.args[2], e);
    }

    EngineRigidBody::H makeRigidBody(Command cmd) {
        if (cmd.args.size() != 3) return nullptr;

        RigidBody e;
        if (!parseRigidBody(cmd, e)) return nullptr;

        return EngineEntity::make<EngineRigidBody>(cmd.args[2], e);
    }

    EngineParticleCollider::H makeParticleCollider(Command cmd) {
        if (cmd.args.size() != 3) return nullptr;

        Particle e;
        if (!parseParticle(cmd, e)) return nullptr;

        return EngineEntity::make<EngineParticleCollider>(cmd.args[2], e);
    }

    EngineSphereCollider::H makeSphereCollider(Command cmd) {
        if (cmd.args.size() != 3) return nullptr;

        RigidBody e;
        if (!parseRigidBody(cmd, e)) return nullptr;

        std::string rStr = cmd.getFlag("radius");
        float r;
        if (!fromString(rStr, r)) return nullptr;
        if (r < 0.0f) return nullptr;

        return EngineEntity::make<EngineSphereCollider>(cmd.args[2], e, r);
    }

    EngineBoxCollider::H makeBoxCollider(Command cmd) {
        if (cmd.args.size() != 3) return nullptr;

        RigidBody e;
        if (!parseRigidBody(cmd, e)) return nullptr;

        std::string sizeStr = cmd.getFlag("size");
        Vec3f size;
        if (!fromString(sizeStr, size)) return nullptr;
        if (size < 0.0f) return nullptr;

        return EngineEntity::make<EngineBoxCollider>(cmd.args[2], e, size);
    }


    void nav(Command cmd) {
        if (cmd.args.size() != 2) return;
        Tree::H next = getTree(cmd.args[1]);
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
        if (mesh->get().getTriangleCount() == 0) return;

        insertTree(mesh);
    }

    void make_texture(Command cmd) {
        if (cmd.args.size() != 3) return;

        std::string filename = cmd.getFlag("filename");

        EngineTexture::H texture = EngineEntity::make<EngineTexture>(
            cmd.args[2], filename, rgb1
        );
        if (texture->get().getSize() == Vec2u{}) return;

        insertTree(texture);
    }

    void make_entity(Command cmd) {
        if (cmd.args.size() != 3) return;

        std::string meshPath = cmd.getFlag("mesh");
        auto mesh = EngineEntity::cast<EngineMesh>(getTree(meshPath));
        if (!mesh) return;

        EnginePhysicsEntity::H phys;
        std::string physType = cmd.getFlag("phys", "static");
        if (physType == "static") {
            Vec3f pos;
            Vec3f rot;
            if (!fromString(cmd.getFlag("pos", "0,0,0"), pos)) return;
            if (!fromString(cmd.getFlag("rot", "0,0,0"), rot)) return;
            phys = EngineEntity::make<EngineStaticPhysics>(cmd.args[2], pos, rot);
        } else if (physType == "particle") {
            phys = makeParticle(cmd);
        } else if (physType == "rigidbody") {
            phys = makeRigidBody(cmd);
        } else if (physType == "particlecollider") {
            phys = makeParticleCollider(cmd);
        } else if (physType == "spherecollider") {
            phys = makeSphereCollider(cmd);
        } else if (physType == "boxcollider") {
            phys = makeBoxCollider(cmd);
        } else return;
        if (!phys) return;

        std::string vsPosStr = cmd.getFlag("vs.pos", "0,0,0");
        Vec3f vsPos;
        if (!fromString(vsPosStr, vsPos)) return;

        std::string vsRotStr = cmd.getFlag("vs.rot", "0,0,0");
        Vec3f vsRot;
        if (!fromString(vsRotStr, vsRot)) return;

        std::string vsScaleStr = cmd.getFlag("vs.scale", "1,1,1");
        Vec3f vsScale;
        if (!fromString(vsScaleStr, vsScale)) return;

        std::string psSpecExponentStr = cmd.getFlag("ps.specexp", "8");
        float psSpecExponent;
        if (!fromString(psSpecExponentStr, psSpecExponent)) return;

        std::string psSpecIntensityStr = cmd.getFlag("ps.specint", "1");
        float psSpecIntensity;
        if (!fromString(psSpecIntensityStr, psSpecIntensity)) return;
        if (psSpecIntensity < 0.0f) return;

        std::unique_ptr<EngineVertexShaderBase> vs;
        std::unique_ptr<EnginePixelShaderBase> ps;
        if (cmd.hasFlag("texture")) {
            auto texture = EngineEntity::cast<EngineTexture>(
                getTree(cmd.getFlag("texture")));
            if (!texture) return;
            vs = std::make_unique<EngineVertexShaderTextured>(
                vsPos, vsRot, vsScale);
            ps = std::make_unique<EnginePixelShaderTextured>(
                texture, psSpecExponent, psSpecIntensity);
        } else {
            std::string psColorStr = cmd.getFlag("color", "255,255,255");
            Vec3f psColor;
            if (!fromString(psColorStr, psColor)) return;
            if (psColor < 0.0f || psColor > 255.0f) return;
            vs = std::make_unique<EngineVertexShader>(
                vsPos, vsRot, vsScale);
            ps = std::make_unique<EnginePixelShader>(
                psColor, psSpecExponent, psSpecIntensity);
        }

        EngineGraphicsEntity::H entity = EngineEntity::make<EngineGraphicsEntity>(
            cmd.args[2], mesh, phys, std::move(vs), std::move(ps)
        );
        if (!entity->get()) return;

        insertTree(entity);
        addTask([entity]() { entity->enable(); });
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
        addTask([light]() { light->enable(); });
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
        addTask([light]() { light->enable(); });
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
        addTask([light]() { light->enable(); });
    }


    void make_particle(Command cmd) {
        auto e = makeParticle(cmd);
        if (!e) return;
        insertTree(e);
        addTask([e]() { e->enable(); });
    }

    void make_rigidbody(Command cmd) {
        auto e = makeRigidBody(cmd);
        if (!e) return;
        insertTree(e);
        addTask([e]() { e->enable(); });
    }

    void make_particlecollider(Command cmd) {
        auto c = makeParticleCollider(cmd);
        if (!c) return;
        insertTree(c);
        addTask([c]() { c->enable(); });
    }

    void make_planecollider(Command cmd) {
        if (cmd.args.size() != 3) return;

        std::string nStr = cmd.getFlag("normal");
        Vec3f n;
        if (!fromString(nStr, n)) return;

        std::string dStr = cmd.getFlag("distance");
        float d;
        if (!fromString(dStr, d)) return;

        auto c = EngineEntity::make<EnginePlaneCollider>(cmd.args[2], n, d);
        insertTree(c);
        addTask([c]() { c->enable(); });
    }

    void make_spherecollider(Command cmd) {
        auto c = makeSphereCollider(cmd);
        if (!c) return;
        insertTree(c);
        addTask([c]() { c->enable(); });
    }

    void make_boxcollider(Command cmd) {
        auto c = makeBoxCollider(cmd);
        if (!c) return;
        insertTree(c);
        addTask([c]() { c->enable(); });
    }

    void make_distanceconstraint(Command cmd) {
        if (cmd.args.size() != 5) return;

        auto e1 = EngineEntity::cast<EngineParticle>(getTree(cmd.args[3]));
        auto e2 = EngineEntity::cast<EngineParticle>(getTree(cmd.args[4]));
        if (!e1 || !e2) return;

        bool visible = cmd.hasFlag("visible");

        std::string lengthStr = cmd.getFlag("length");
        float l0;
        if (!fromString(lengthStr, l0)) return;
        if (l0 < 0.0f) return;

        std::string complianceStr = cmd.getFlag("compliance", "0");
        float a;
        if (!fromString(complianceStr, a)) return;
        if (a < 0.0f) return;

        auto c = EngineEntity::make<EngineDistanceConstraint>(
            cmd.args[2],
            visible,
            DistanceConstraint(l0, a, &e1->get(), &e2->get())
        );
        insertTree(c);
        addTask([c]() { c->enable(); });
    }

    void make_ropeconstraint(Command cmd) {
        if (cmd.args.size() != 5) return;

        auto e1 = EngineEntity::cast<EngineRigidBody>(getTree(cmd.args[3]));
        auto e2 = EngineEntity::cast<EngineRigidBody>(getTree(cmd.args[4]));
        if (!e1 || !e2) return;

        bool visible = cmd.hasFlag("visible");

        std::string lengthStr = cmd.getFlag("length");
        float l0;
        if (!fromString(lengthStr, l0)) return;
        if (l0 < 0.0f) return;

        std::string complianceStr = cmd.getFlag("compliance", "0");
        float a;
        if (!fromString(complianceStr, a)) return;
        if (a < 0.0f) return;

        std::string posStr1 = cmd.getFlag("pos1", "0,0,0");
        Vec3f r1;
        if (!fromString(posStr1, r1)) return;

        std::string posStr2 = cmd.getFlag("pos2", "0,0,0");
        Vec3f r2;
        if (!fromString(posStr2, r2)) return;

        auto c = EngineEntity::make<EngineRopeConstraint>(
            cmd.args[2],
            visible,
            RopeConstraint(l0, a, &e1->get(), r1, &e2->get(), r2)
        );
        insertTree(c);
        addTask([c]() { c->enable(); });
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


    void get_prop(Command cmd) {
        if (cmd.args.size() != 3) return;
        
        assert(dir);
        EngineEntity::H e = EngineEntity::cast<EngineEntity>(dir);
        if (!e) return;

        std::cout << e->getProperty(cmd.args[2]) << '\n';
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


    void set_prop(Command cmd) {
        if (cmd.args.size() != 4) return;

        assert(dir);
        EngineEntity::H e = EngineEntity::cast<EngineEntity>(dir);
        if (!e) return;

        addTask([e, cmd]() { e->setProperty(cmd.args[2], cmd.args[3]); });
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
        addTask([tree]() { EngineEntity::disableAll(tree); });
    }


    void sim_start(Command cmd) {
        if (cmd.args.size() != 2) return;
        addTask([]() {
            rootSnap = EngineEntity::copyAll(root);
            dirSnap = rootSnap->getRelative(dir->getPath().substr(dir == root ? 5 : 6));
            simStart();
        });
    }

    void sim_stop(Command cmd) {
        if (cmd.args.size() != 2) return;
        addTask([]() {
            EngineEntity::disableAll(root);
            EngineEntity::enableAll(rootSnap);
            root = std::move(rootSnap);
            dir = std::move(dirSnap);
            simStop();
        });
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
        cmdTreeMake->insert(CommandNode::make("particle", make_particle));
        cmdTreeMake->insert(CommandNode::make("rigidbody", make_rigidbody));
        cmdTreeMake->insert(CommandNode::make("particlecollider", make_particlecollider));
        cmdTreeMake->insert(CommandNode::make("planecollider", make_planecollider));
        cmdTreeMake->insert(CommandNode::make("spherecollider", make_spherecollider));
        cmdTreeMake->insert(CommandNode::make("boxcollider", make_boxcollider));
        cmdTreeMake->insert(CommandNode::make("distanceconstraint", make_distanceconstraint));
        cmdTreeMake->insert(CommandNode::make("ropeconstraint", make_ropeconstraint));
        cmdTree->insert(cmdTreeMake);

        Tree::H cmdTreeGet = Tree::make("get");
        cmdTreeGet->insert(CommandNode::make("path", get_path));
        cmdTreeGet->insert(CommandNode::make("name", get_name));
        cmdTreeGet->insert(CommandNode::make("kids", get_kids));
        cmdTreeGet->insert(CommandNode::make("prop", get_prop));
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
        cmdTreeSet->insert(CommandNode::make("prop", set_prop));
        cmdTreeSet->insert(CommandNode::make("cameraRes", set_cameraRes));
        cmdTreeSet->insert(CommandNode::make("cameraPos", set_cameraPos));
        cmdTreeSet->insert(CommandNode::make("cameraRot", set_cameraRot));
        cmdTreeSet->insert(CommandNode::make("cameraFov", set_cameraFov));
        cmdTreeSet->insert(CommandNode::make("cameraFocalLength", set_cameraFocalLength));
        cmdTreeSet->insert(CommandNode::make("ambientLight", set_ambientLight));
        cmdTreeSet->insert(CommandNode::make("backgroundColor", set_backgroundColor));
        cmdTree->insert(cmdTreeSet);

        cmdTree->insert(CommandNode::make("del", del));

        Tree::H cmdTreeSim = Tree::make("sim");
        cmdTreeSim->insert(CommandNode::make("start", sim_start));
        cmdTreeSim->insert(CommandNode::make("stop", sim_stop));
        cmdTree->insert(cmdTreeSim);

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

    Tree::H getTree(std::string path) {
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
