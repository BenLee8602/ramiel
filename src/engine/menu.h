#pragma once

namespace ramiel {

    class Menu {
    public:
        static Menu* getCurrent();
        static Menu* clearCurrent();

        static void currentRender();
        static void currentControls();

        virtual void render() const = 0;
        virtual void controls() const = 0;

        bool isCurrent() const;
        Menu* makeCurrent();

        virtual ~Menu();
    };

}

