#pragma once

#include <string>

namespace ramiel {

    void termInit();
    void termExit();

    void termOpen();
    void termClose();

    std::string termRead();
    void termWrite(const std::string& msg);

    class Menu;
    Menu* getTerminal();

}

