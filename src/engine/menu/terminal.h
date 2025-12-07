#pragma once

#include <string>
#include <functional>

namespace ramiel {

    using TermReadCallback = std::function<void(const std::string&)>;

    bool termIsOpen();
    void termOpen();
    void termClose();

    void termWrite(const std::string& msg);
    void termRead(TermReadCallback cb);

    class Menu;
    Menu* getTerminal();

}

