#include <iostream>
#include <unordered_map>
#include "stringify.h"

namespace {

    using TypeMap = std::unordered_map<std::string, std::string>;
    using SizeMap = std::unordered_map<std::string, uint8_t>;

    const TypeMap vertexTypeMap = {
        { "Scalar", "float" },
        { "Vec2", "ramiel::Vec2f" },
        { "Vec3", "ramiel::Vec3f" },
        { "Vec4", "ramiel::Vec4f" }
    };

    const TypeMap stateTypeMap = {
        { "Scalar", "float" },
        { "Vec2", "ramiel::Vec2f" },
        { "Vec3", "ramiel::Vec3f" },
        { "Vec4", "ramiel::Vec4f" },
        { "Mat2x2", "ramiel::Mat2x2f" },
        { "Mat2x3", "ramiel::Mat2x3f" },
        { "Mat2x4", "ramiel::Mat2x4f" },
        { "Mat3x2", "ramiel::Mat3x2f" },
        { "Mat3x3", "ramiel::Mat3x3f" },
        { "Mat3x4", "ramiel::Mat3x4f" },
        { "Mat4x2", "ramiel::Mat4x2f" },
        { "Mat4x3", "ramiel::Mat4x3f" },
        { "Mat4x4", "ramiel::Mat4x4f" },
        { "Buffer", "std::shared_ptr<ramiel::Texture>" }
    };

    const SizeMap sizeMap = {
        { "Scalar", 1 },
        { "Vec2", 2 },
        { "Vec3", 3 },
        { "Vec4", 4 }
    };

    std::string getType(const TypeMap& typeMap, const std::string& type) {
        auto res = typeMap.find(type);
        if (res == typeMap.end()) {
            std::cout << "error: invalid type \"" << type << "\"\n";
            exit(1);
        }
        return res->second;
    }
    
}

namespace ramiel::shaderlang {

    std::string stringifyState(const std::vector<Attr>& attr) {
        std::string out;
        for (auto& a : attr) {
            out += getType(stateTypeMap, a.type) + ' ' + a.name + ';';
        }
        return out;
    }


    std::string stringifyCtorArgs(const std::vector<Attr>& attr) {
        std::string out;
        for (auto& a : attr) {
            out += "const " + getType(stateTypeMap, a.type) + "& " + a.name + ',';
        }
        out.pop_back();
        return out;
    }


    std::string stringifyCtorInit(const std::vector<Attr>& attr) {
        std::string out;
        for (auto& a : attr) {
            out += a.name + '(' + a.name + "),";
        }
        out.pop_back();
        return out;
    }


    std::string stringifyAttrType(const std::vector<Attr>& attr) {
        std::string out = "{";
        for (auto& a : attr) {
            out += std::to_string(sizeMap.find(a.type)->second) + ',';
        }
        out.pop_back();
        return out + '}';
    }


    std::string stringifyAttrOutPos(const std::vector<Attr>& attr) {
        uint8_t pos = 0;
        std::string out = "{";
        for (auto& a : attr) {
            out += std::to_string(pos) + ',';
            pos += sizeMap.find(a.type)->second;
        }
        out.pop_back();
        return out + '}';
    }


    std::string stringifyAttrOutSize(const std::vector<Attr>& attr) {
        uint8_t size = 0;
        for (auto& a : attr) {
            size += sizeMap.find(a.type)->second;
        }
        return std::to_string(size);
    }


    std::string stringifyAttrInDef(const std::vector<Attr>& attr) {
        std::string out;
        for (auto& a : attr) {
            out += "const " + getType(vertexTypeMap, a.type) + "& " + a.name + ';';
        }
        return out;
    }


    std::string stringifyAttrOutDef(const std::vector<Attr>& attr) {
        std::string out;
        for (auto& a : attr) {
            out += getType(vertexTypeMap, a.type) + ' ' + a.name + ';';
        }
        return out;
    }


    std::string stringifyAttrInInterpret(const std::vector<Attr>& attr) {
        std::string out;
        for (size_t i = 0; i < attr.size(); i++) {
            out += (
                "reinterpret_cast<const " +
                getType(vertexTypeMap, attr[i].type) +
                "&>(*(v + attrInPos[" +
                std::to_string(i) +
                "])),"
            );
        }
        out.pop_back();
        return out;
    }

}
