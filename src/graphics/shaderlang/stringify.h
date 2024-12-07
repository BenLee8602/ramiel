#pragma once

#include <string>
#include "attribute.h"

namespace ramiel::shaderlang {

    std::string stringifyState(const std::vector<Attr>& attr);
    std::string stringifyCtorArgs(const std::vector<Attr>& attr);
    std::string stringifyCtorInit(const std::vector<Attr>& attr);
    std::string stringifyAttrType(const std::vector<Attr>& attr);
    std::string stringifyAttrOutPos(const std::vector<Attr>& attr);
    std::string stringifyAttrOutSize(const std::vector<Attr>& attr);
    std::string stringifyAttrInDef(const std::vector<Attr>& attr);
    std::string stringifyAttrOutDef(const std::vector<Attr>& attr);
    std::string stringifyAttrInInterpret(const std::vector<Attr>& attr);

}
