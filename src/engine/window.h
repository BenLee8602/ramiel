#pragma once

#include <cstdint>

namespace ramiel {

    enum class Key : uint8_t {
        MOUSELEFT, MOUSEMIDDLE, MOUSERIGHT, // mouse
        ESCAPE, BACKSPACE, TAB, ENTER, SPACE, // special
        SHIFT, CONTROL, ALT, // modifier
        LEFT, RIGHT, DOWN, UP, // navigation
        ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, // numeric
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, // alphabetic
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, // function
        TICK, MINUS, PLUS, BRACKETOPEN, BRACKETCLOSE, BACKSLASH, COLON, QUOTE, COMMA, PERIOD, SLASH // misc
    };

    void initWindow();
    void destroyWindow();

    bool windowGood();
    void pollWindowMessages();

    Vec2u getWindowSize();
    void setWindowSize(Vec2u size);

    bool keyDown(Key key);
    bool keyPressed(Key key);
    bool keyUp(Key key);

    void updateFrame();

}
