#include <utility>

#include "menu.h"
#include "graphics.h"
using namespace ramiel;

namespace {

    Menu* curMenu = nullptr;

}

namespace ramiel {

    Menu* Menu::getCurrent() {
        return curMenu;
    }

    Menu* Menu::clearCurrent() {
        menuRenderNeeded();
        return std::exchange(curMenu, nullptr);
    }


    void Menu::currentRender() {
        if (curMenu) curMenu->render();
    }

    void Menu::currentControls() {
        if (curMenu) curMenu->controls();
    }


    bool Menu::isCurrent() const {
        return this == curMenu;
    }

    Menu* Menu::makeCurrent() {
        menuRenderNeeded();
        return std::exchange(curMenu, this);
    }


    Menu::~Menu() {
        if (isCurrent()) Menu::clearCurrent();
    }

}

