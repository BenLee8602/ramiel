#include <windows.h>
#undef min
#undef max

#include <cassert>
#include <vector>

#include <ramiel/graphics.h>
#include "window.h"
using namespace ramiel;

namespace {

    const std::unordered_map<int, Key> keyMap = {
        { VK_LBUTTON, Key::MOUSELEFT },
        { VK_MBUTTON, Key::MOUSEMIDDLE },
        { VK_RBUTTON, Key::MOUSERIGHT },

        { VK_ESCAPE, Key::ESCAPE },
        { VK_BACK,   Key::BACKSPACE },
        { VK_TAB,    Key::TAB },
        { VK_RETURN, Key::ENTER },
        { VK_SPACE,  Key::SPACE },

        { VK_SHIFT,   Key::SHIFT },
        { VK_CONTROL, Key::CONTROL },
        { VK_MENU,    Key::ALT },

        { VK_LEFT,  Key::LEFT },
        { VK_RIGHT, Key::RIGHT },
        { VK_DOWN,  Key::DOWN },
        { VK_UP,    Key::UP },

        { static_cast<int>('0'), Key::ZERO },
        { static_cast<int>('1'), Key::ONE },
        { static_cast<int>('2'), Key::TWO },
        { static_cast<int>('3'), Key::THREE },
        { static_cast<int>('4'), Key::FOUR },
        { static_cast<int>('5'), Key::FIVE },
        { static_cast<int>('6'), Key::SIX },
        { static_cast<int>('7'), Key::SEVEN },
        { static_cast<int>('8'), Key::EIGHT },
        { static_cast<int>('9'), Key::NINE },

        { static_cast<int>('A'), Key::A },
        { static_cast<int>('B'), Key::B },
        { static_cast<int>('C'), Key::C },
        { static_cast<int>('D'), Key::D },
        { static_cast<int>('E'), Key::E },
        { static_cast<int>('F'), Key::F },
        { static_cast<int>('G'), Key::G },
        { static_cast<int>('H'), Key::H },
        { static_cast<int>('I'), Key::I },
        { static_cast<int>('J'), Key::J },
        { static_cast<int>('K'), Key::K },
        { static_cast<int>('L'), Key::L },
        { static_cast<int>('M'), Key::M },
        { static_cast<int>('N'), Key::N },
        { static_cast<int>('O'), Key::O },
        { static_cast<int>('P'), Key::P },
        { static_cast<int>('Q'), Key::Q },
        { static_cast<int>('R'), Key::R },
        { static_cast<int>('S'), Key::S },
        { static_cast<int>('T'), Key::T },
        { static_cast<int>('U'), Key::U },
        { static_cast<int>('V'), Key::V },
        { static_cast<int>('W'), Key::W },
        { static_cast<int>('X'), Key::X },
        { static_cast<int>('Y'), Key::Y },
        { static_cast<int>('Z'), Key::Z },

        { VK_F1,  Key::F1 },
        { VK_F2,  Key::F2 },
        { VK_F3,  Key::F3 },
        { VK_F4,  Key::F4 },
        { VK_F5,  Key::F5 },
        { VK_F6,  Key::F6 },
        { VK_F7,  Key::F7 },
        { VK_F8,  Key::F8 },
        { VK_F9,  Key::F9 },
        { VK_F10, Key::F10 },
        { VK_F11, Key::F11 },
        { VK_F12, Key::F12 },

        { VK_OEM_3,      Key::TICK },
        { VK_OEM_MINUS,  Key::MINUS },
        { VK_OEM_PLUS,   Key::PLUS },
        { VK_OEM_4,      Key::BRACKETOPEN },
        { VK_OEM_6,      Key::BRACKETCLOSE },
        { VK_OEM_5,      Key::BACKSLASH },
        { VK_OEM_1,      Key::COLON },
        { VK_OEM_7,      Key::QUOTE },
        { VK_OEM_COMMA,  Key::COMMA },
        { VK_OEM_PERIOD, Key::PERIOD },
        { VK_OEM_2,      Key::SLASH }
    };

    std::vector<bool> prevKeyStates(keyMap.size());
    std::vector<bool> keyStates(keyMap.size());


    bool init = false;
    bool good = true;

    HWND window;
    HDC deviceContext;
    BITMAPINFO frameInfo;

    constexpr int defaultWidth = 1280;
    constexpr int defaultHeight = 720;


    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg == WM_DESTROY) {
            good = false;
            PostQuitMessage(0);
            return 0;
        }
        if (uMsg == WM_KEYDOWN) {
            auto keyIt = keyMap.find(wParam);
            if (keyIt != keyMap.end()) {
                uint8_t key = static_cast<uint8_t>(keyIt->second);
                keyStates[key] = true;
            }
            return 0;
        }
        if (uMsg == WM_KEYUP) {
            auto keyIt = keyMap.find(wParam);
            if (keyIt != keyMap.end()) {
                uint8_t key = static_cast<uint8_t>(keyIt->second);
                keyStates[key] = false;
            }
            return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

}

namespace ramiel {

    bool initWindow() {
        if (init) return false;
        init = true;

        const char* windowClassName = "WindowClass";

        WNDCLASS windowClass = {};
        windowClass.lpfnWndProc = WindowProc;
        windowClass.lpszClassName = windowClassName;
        RegisterClass(&windowClass);

        window = CreateWindowEx(
            0,
            windowClassName,
            "Ramiel Engine",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            defaultWidth,
            defaultHeight,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        );
        if (!window) return false;

        deviceContext = GetDC(window);

        frameInfo = {};
        frameInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        frameInfo.bmiHeader.biWidth = defaultWidth;
        frameInfo.bmiHeader.biHeight = defaultHeight;
        frameInfo.bmiHeader.biPlanes = 1;
        frameInfo.bmiHeader.biBitCount = 24;
        frameInfo.bmiHeader.biCompression = BI_RGB;

        ShowWindow(window, SW_SHOW);
        return true;
    }


    bool windowGood() {
        return init && good;
    }


    void pollWindowMessages() {
        assert(init && good);

        prevKeyStates = keyStates;

        MSG msg = {};
        while (PeekMessage(&msg, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    size_t windowWidth() {
        assert(init && good);
        return frameInfo.bmiHeader.biWidth;
    }

    size_t windowHeight() {
        assert(init && good);
        return frameInfo.bmiHeader.biHeight;
    }


    bool keyDown(Key key_) {
        assert(init && good);
        uint8_t key = static_cast<uint8_t>(key_);
        return !prevKeyStates[key] && keyStates[key];
    }

    bool keyPressed(Key key_) {
        assert(init && good);
        uint8_t key = static_cast<uint8_t>(key_);
        return keyStates[key];
    }

    bool keyUp(Key key_) {
        assert(init && good);
        uint8_t key = static_cast<uint8_t>(key_);
        return prevKeyStates[key] && !keyStates[key];
    }


    void drawToWindow(uint8_t* frame) {
        assert(init && good);
        assert(frame);

        int width = frameInfo.bmiHeader.biWidth;
        int height = frameInfo.bmiHeader.biHeight;

        SetDIBitsToDevice(
            deviceContext,
            0, 0,
            width, height,
            0, 0,
            0, height,
            frame,
            &frameInfo,
            DIB_RGB_COLORS
        );
    }

}
