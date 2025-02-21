#pragma once

#include <string>

namespace ramiel::shaderlang {

    void tokenize(const std::string& in);
    
    const std::string& getToken();
    const std::string& popToken();
    bool tryPopToken(std::string& out);

}
