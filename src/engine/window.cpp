#include <SDL3/SDL.h>

#include <cassert>
#include <vector>

#include <ramiel/graphics.h>
#include "window.h"
#include "graphics.h"
using namespace ramiel;

namespace {

    const std::unordered_map<SDL_Keycode, Key> keyMap = {
        { SDL_BUTTON_LEFT,   Key::MOUSELEFT   },
        { SDL_BUTTON_MIDDLE, Key::MOUSEMIDDLE },
        { SDL_BUTTON_RIGHT,  Key::MOUSERIGHT  },

        { SDLK_ESCAPE,    Key::ESCAPE    },
        { SDLK_BACKSPACE, Key::BACKSPACE },
        { SDLK_TAB,       Key::TAB       },
        { SDLK_RETURN,    Key::ENTER     },
        { SDLK_SPACE,     Key::SPACE     },

        { SDLK_LSHIFT, Key::SHIFT   },
        { SDLK_RSHIFT, Key::SHIFT   },
        { SDLK_LCTRL,  Key::CONTROL },
        { SDLK_RCTRL,  Key::CONTROL },
        { SDLK_LALT,   Key::ALT     },
        { SDLK_RALT,   Key::ALT     },

        { SDLK_LEFT,  Key::LEFT  },
        { SDLK_RIGHT, Key::RIGHT },
        { SDLK_DOWN,  Key::DOWN  },
        { SDLK_UP,    Key::UP    },

        { SDLK_0, Key::ZERO  },
        { SDLK_1, Key::ONE   },
        { SDLK_2, Key::TWO   },
        { SDLK_3, Key::THREE },
        { SDLK_4, Key::FOUR  },
        { SDLK_5, Key::FIVE  },
        { SDLK_6, Key::SIX   },
        { SDLK_7, Key::SEVEN },
        { SDLK_8, Key::EIGHT },
        { SDLK_9, Key::NINE  },

        { SDLK_A, Key::A },
        { SDLK_B, Key::B },
        { SDLK_C, Key::C },
        { SDLK_D, Key::D },
        { SDLK_E, Key::E },
        { SDLK_F, Key::F },
        { SDLK_G, Key::G },
        { SDLK_H, Key::H },
        { SDLK_I, Key::I },
        { SDLK_J, Key::J },
        { SDLK_K, Key::K },
        { SDLK_L, Key::L },
        { SDLK_M, Key::M },
        { SDLK_N, Key::N },
        { SDLK_O, Key::O },
        { SDLK_P, Key::P },
        { SDLK_Q, Key::Q },
        { SDLK_R, Key::R },
        { SDLK_S, Key::S },
        { SDLK_T, Key::T },
        { SDLK_U, Key::U },
        { SDLK_V, Key::V },
        { SDLK_W, Key::W },
        { SDLK_X, Key::X },
        { SDLK_Y, Key::Y },
        { SDLK_Z, Key::Z },

        { SDLK_F1,  Key::F1  },
        { SDLK_F2,  Key::F2  },
        { SDLK_F3,  Key::F3  },
        { SDLK_F4,  Key::F4  },
        { SDLK_F5,  Key::F5  },
        { SDLK_F6,  Key::F6  },
        { SDLK_F7,  Key::F7  },
        { SDLK_F8,  Key::F8  },
        { SDLK_F9,  Key::F9  },
        { SDLK_F10, Key::F10 },
        { SDLK_F11, Key::F11 },
        { SDLK_F12, Key::F12 },

        { SDLK_GRAVE,        Key::TICK         },
        { SDLK_MINUS,        Key::MINUS        },
        { SDLK_EQUALS,       Key::PLUS         },
        { SDLK_LEFTBRACKET,  Key::BRACKETOPEN  },
        { SDLK_RIGHTBRACKET, Key::BRACKETCLOSE },
        { SDLK_BACKSLASH,    Key::BACKSLASH    },
        { SDLK_SEMICOLON,    Key::COLON        },
        { SDLK_APOSTROPHE,   Key::QUOTE        },
        { SDLK_COMMA,        Key::COMMA        },
        { SDLK_PERIOD,       Key::PERIOD       },
        { SDLK_SLASH,        Key::SLASH        }
    };


    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;

    std::vector<bool> prevKeyStates(keyMap.size());
    std::vector<bool> keyStates(keyMap.size());


    InputField* field = nullptr;
    size_t token = 0;


    SDL_Texture* makeTexture(Vec2u size) {
        assert(renderer);
        return SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            size[X], size[Y]
        );

    }

    void handleKeyEvent(SDL_Keycode keyCode, bool value) {
        auto keyIt = keyMap.find(keyCode);
        if (keyIt == keyMap.end()) return;
        Key key = keyIt->second;

        if (!field) {
            keyStates[static_cast<uint8_t>(key)] = value;
            return;
        }

        if (!value) return;
        if (key == Key::ESCAPE) {
            InputField::clearCurrent();
        } else if (key == Key::ENTER) {
            field->getOnSubmit()(field->getValue());
        } else if (key == Key::BACKSPACE && token > 0) {
            std::string fieldVal = field->getValue();
            fieldVal.erase(--token, 1);
            field->setValue(fieldVal);
        } else if (key == Key::LEFT) {
            if (token > 0) token--;
        } else if (key == Key::RIGHT) {
            if (token < field->getValue().size()) token++;
        }
    }


    void onWindowResized() {
        assert(window);
        assert(texture);

        Vec2u size = getWindowSize();

        resizeFrame(size);

        SDL_DestroyTexture(texture);
        texture = makeTexture(size);
    }


    void insertToField(char c) {
        assert(field);
        std::string value = field->getValue();
        value.insert(token, &c, 1);
        if (field->setValue(value)) token++;
    }

}

namespace ramiel {

    void initWindow() {
        if (window) return;
        assert(!window);
        assert(!renderer);
        assert(!texture);

        if (SDL_Init(SDL_INIT_VIDEO) < 0) return;

        constexpr Vec2u defaultSize = { 800, 600 };

        resizeFrame(defaultSize);

        window = SDL_CreateWindow(
            "ramiel engine",
            defaultSize[X], defaultSize[Y],
            SDL_WINDOW_OPENGL
        );
        if (!window) {
            SDL_Quit();
            return;
        }

        renderer = SDL_CreateRenderer(window, nullptr);
        if (!renderer) {
            SDL_DestroyWindow(window);
            SDL_Quit();
            window = nullptr;
            return;
        }

        texture = makeTexture(defaultSize);
        assert(texture);
    }


    void destroyWindow() {
        if (!window) return;
        assert(window);
        assert(renderer);
        assert(texture);

        InputField::clearCurrent();

        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

        texture = nullptr;
        renderer = nullptr;
        window = nullptr;
    }


    bool windowGood() {
        return window;
    }


    void pollWindowMessages() {
        assert(window);

        prevKeyStates = keyStates;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                destroyWindow();
                return;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                handleKeyEvent(event.key.key, true);
            } else if (event.type == SDL_EVENT_KEY_UP) {
                handleKeyEvent(event.key.key, false);
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                handleKeyEvent(event.button.button, true);
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                handleKeyEvent(event.button.button, false);
            } else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                onWindowResized();
            } else if (event.type == SDL_EVENT_TEXT_INPUT) {
                insertToField(event.text.text[0]);
            }
        }
    }


    Vec2u getWindowSize() {
        assert(window);
        int w, h;
        SDL_GetWindowSizeInPixels(window, &w, &h);
        return {
            static_cast<unsigned>(w),
            static_cast<unsigned>(h)
        };
    }

    void setWindowSize(Vec2u size) {
        assert(window);
        SDL_SetWindowSize(window, size[X], size[Y]);
    }


    bool keyDown(Key key_) {
        assert(window);
        uint8_t key = static_cast<uint8_t>(key_);
        return !prevKeyStates[key] && keyStates[key];
    }

    bool keyPressed(Key key_) {
        assert(window);
        uint8_t key = static_cast<uint8_t>(key_);
        return keyStates[key];
    }

    bool keyUp(Key key_) {
        assert(window);
        uint8_t key = static_cast<uint8_t>(key_);
        return prevKeyStates[key] && !keyStates[key];
    }


    void updateFrame() {
        assert(window);
        assert(renderer);
        assert(texture);

        void* txpixels = nullptr;
        int txpitch = 0;
        SDL_LockTexture(texture, nullptr, &txpixels, &txpitch);

        uint8_t* in  = static_cast<uint8_t*>(getColorBuffer());
        uint8_t* out = static_cast<uint8_t*>(txpixels);
        Vec2u size = getRes();

        for (size_t y = 0; y < size[Y]; y++) {
            uint8_t* o = out + txpitch * (size[Y] - y - 1);
            for (size_t x = 0; x < size[X]; x++) {
                o[2] = std::min<uint8_t>(*in++, 255);
                o[1] = std::min<uint8_t>(*in++, 255);
                o[0] = std::min<uint8_t>(*in++, 255);
                o[3] = 255;
                o += 4;
            }
        }

        SDL_UnlockTexture(texture);

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }


    InputField* InputField::getCurrent() {
        return field;
    }

    void InputField::clearCurrent() {
        assert(window);
        if (!field) return;
        SDL_StopTextInput(window);
        field = nullptr;
        token = 0;
    }


    InputField::InputField(
        const std::string& value,
        OnChange onChange,
        OnSubmit onSubmit
    ) {
        setOnChange(onChange);
        setOnSubmit(onSubmit);
        setValue(value);
    }


    InputField::~InputField() {
        if (isCurrent()) InputField::clearCurrent();
    }


    bool InputField::isCurrent() const {
        return field == this;
    }

    void InputField::makeCurrent() {
        assert(window);
        if (!field) {
            SDL_StartTextInput(window);
            std::fill(keyStates.begin(), keyStates.end(), false);
        }
        field = this;
        token = value.size();
    }


    const std::string& InputField::getValue() const {
        return value;
    }

    bool InputField::setValue(const std::string& value) {
        if (!onChange(value)) return false;
        this->value = value;
        return true;
    }


    InputField::OnChange InputField::getOnChange() const {
        return onChange;
    }

    void InputField::setOnChange(OnChange onChange) {
        static OnChange dflt = [](const std::string&) { return true; };
        this->onChange = onChange ? onChange : dflt;
    }


    InputField::OnSubmit InputField::getOnSubmit() const {
        return onSubmit;
    }

    void InputField::setOnSubmit(OnSubmit onSubmit) {
        static OnSubmit dflt = [](const std::string&) {};
        this->onSubmit = onSubmit ? onSubmit : dflt;
    }

}

