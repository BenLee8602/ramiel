#pragma once

#include <vector>
#include <string>
#include <memory>

#include "entity.h"

namespace ramiel {

    std::string pwd();
    bool cd(std::string path);
    std::vector<std::string> ls();
    EngineEntity* get(std::string path = "");
    bool mkdir(std::string name);
    void add(std::string name, std::unique_ptr<EngineEntity>&& e);
    bool rm(std::string name);

    template<class EngineEntityType>
    EngineEntityType* get(std::string path = "") {
        return dynamic_cast<EngineEntityType*>(get("path"));
    }

}
