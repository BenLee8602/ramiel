#include <list>

#include "terminal.h"
#include "menu.h"
#include "window.h"
#include "graphics.h"
using namespace ramiel;

namespace {

    bool onTermInputChange(const std::string&);
    void onTermInputSubmit(const std::string&);


    class TerminalMenu : public Menu {
    public:
        static TerminalMenu& get();
        virtual void render() const override;
        virtual void controls() const override;
    private:
        TerminalMenu();
    };


    bool isOpen = false;
    Menu* prevMenu = nullptr;
    InputField* prevInput = nullptr;

    TextArea text;
    InputField input;

    std::list<std::string> cmdHistory = { std::string() };
    auto curCmd = cmdHistory.begin();

    Vec2u size = {};
    size_t lineStep = 4;


    void updateTextAreaSize() {
        static Vec2f res = {};
        if (res == getRes()) return;
        res = getRes();
        text.setSize(res - text.getFontSize() * 2.0f);
    }


    TerminalMenu::TerminalMenu() {
        static Font font = loadttf(
            ramiel_ENGINE_ASSET_DIR
            "/fonts/jetbrainsmono.ttf"
        );
        constexpr float fontSize = 18;
        constexpr Vec3f fontColor = { 255, 255, 255 };
        constexpr Vec2f pos = { fontSize, fontSize };

        text.setFont(&font);
        text.setFontSize(fontSize);
        text.setFontColor(fontColor);

        text.setPos(pos);
        updateTextAreaSize();

        input.setOnChange(onTermInputChange);
        input.setOnSubmit(onTermInputSubmit);
    }


    TerminalMenu& TerminalMenu::get() {
        static TerminalMenu menu;
        return menu;
    }


    void TerminalMenu::render() const {
        constexpr Vec4ui8 bg = { 0, 0, 0, 200 };
        Vec4ui8* menuBuf = getMenuBuf();
        std::fill(menuBuf, menuBuf + getBufferSize(), bg);

        updateTextAreaSize();
        text.render();
    }


    void TerminalMenu::controls() const {
        if (keyDown(Key::ESCAPE)) {
            termClose();
        }

        if (keyPressed(Key::CONTROL) && keyPressed(Key::SHIFT)) {
            if (keyDown(Key::DOWN) && lineStep > 1) {
                lineStep--;
            }

            if (keyDown(Key::UP)) {
                lineStep++;
            }
        }

        else if (keyPressed(Key::SHIFT)) {
            size_t curLine = text.getLine();
            if (keyDown(Key::DOWN)) {
                curLine += lineStep;

                int nScreen = text.getSize()[Y] / text.getFontSize();
                int nLine = text.getNLines();
                size_t maxLine = std::max<size_t>(0, nLine - nScreen);

                text.setLine(std::min(curLine, maxLine));
                menuRenderNeeded();
            }

            if (keyDown(Key::UP)) {
                curLine = curLine < lineStep ? 0 : curLine - lineStep;
                text.setLine(curLine);
                menuRenderNeeded();
            }
        }

        else {
            if (keyDown(Key::DOWN)) {
                if (curCmd != std::prev(cmdHistory.end())) {
                    curCmd++;
                    input.setValue(*curCmd);
                    input.setToken(curCmd->size());
                }
            }

            if (keyDown(Key::UP)) {
                if (curCmd != cmdHistory.begin()) {
                    curCmd--;
                    input.setValue(*curCmd);
                    input.setToken(curCmd->size());
                }
            }
        }

        input.controls();
    }


    bool onTermInputChange(const std::string& v) {
        size_t n = input.getValue().size();
        text.erase(text.getValue().size() - n, n);
        text.insert(v);
        text.goToBottom();

        menuRenderNeeded();
        return true;
    }

    void onTermInputSubmit(const std::string& v) {
        auto newCmd = std::prev(cmdHistory.end());
        *newCmd = v;

        cmdHistory.emplace_back();
        curCmd = std::prev(cmdHistory.end());

        input.setValue("");
        text.insert(*newCmd + '\n');
        text.goToBottom();

        menuRenderNeeded();
    }

}

namespace ramiel {

    bool termIsOpen() {
        return isOpen;
    }

    void termOpen() {
        isOpen = true;
        prevMenu = TerminalMenu::get().makeCurrent();
        prevInput = input.makeCurrent();
    }

    void termClose() {
        isOpen = false;
        prevMenu ? prevMenu->makeCurrent() : Menu::clearCurrent();
        prevInput ? prevInput->makeCurrent() : InputField::clearCurrent();
    }


    void termWrite(const std::string& msg) {

    }


    void termRead(TermReadCallback cb) {

    }


    Menu* getTerminal() {
        return &TerminalMenu::get();
    }

}



