#include <list>
#include <thread>
#include <atomic>

#include "terminal.h"
#include "menu.h"
#include "window.h"
#include "graphics.h"
#include "command.h"
#include "task.h"
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


    std::thread termThread;
    bool isRunning = false;

    bool isOpen = false;

    Menu* prevMenu = nullptr;
    InputField* prevInput = nullptr;

    TextArea text;
    InputField input;

    std::list<std::string> cmdHistory = { std::string() };
    auto curCmd = cmdHistory.begin();

    Vec2u size = {};
    size_t lineStep = 4;

    std::atomic<bool> isReading = false;
    std::string readVal;


    void updateTextAreaSize() {
        static Vec2f res = {};
        if (res == getRes()) return;
        res = getRes();
        text.setSize(res - text.getFontSize() * 2.0f);
    }


    size_t getMaxLine() {
        int nScreen = text.getSize()[Y] / text.getFontSize();
        int nLine = text.getNLines();
        return std::max<size_t>(0, nLine - nScreen);
    }


    TerminalMenu::TerminalMenu() {
        static Font font = loadttf(
            ramiel_ENGINE_ASSET_DIR
            "/fonts/jetbrainsmono.ttf"
        );
        constexpr float fontSize = 18;
        constexpr Vec3f fontColor = { 235, 220, 180 };
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
        assert(isRunning);
        assert(isOpen);

        constexpr Vec4ui8 bg = { 0, 0, 0, 200 };
        Vec4ui8* menuBuf = getMenuBuf();
        std::fill(menuBuf, menuBuf + getBufferSize(), bg);

        updateTextAreaSize();
        text.render();
    }


    void TerminalMenu::controls() const {
        assert(isRunning);
        assert(isOpen);

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
                text.setLine(std::min(curLine, getMaxLine()));
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
        assert(isRunning);
        assert(isOpen);

        size_t n = input.getValue().size();
        text.erase(text.getValue().size() - n, n);
        text.insert(v);
        text.goToBottom();

        menuRenderNeeded();
        return true;
    }

    void onTermInputSubmit(const std::string& v) {
        assert(isRunning);
        assert(isOpen);
        assert(isReading);

        readVal = v;
        InputField::clearCurrent();

        input.setValue("");
        text.insert(readVal + '\n');
        text.goToBottom();

        isReading = false;
        isReading.notify_one();

        menuRenderNeeded();
    }


    void terminalMain() {
        assert(isRunning);

        while (isRunning) {
            termWrite(getPath() + "> ");
            std::string cmd = termRead();
            if (cmd.empty()) continue;
            runCommand(cmd);

            addTask([cmd]() {
                auto newCmd = std::prev(cmdHistory.end());
                *newCmd = cmd;
                cmdHistory.emplace_back();
                curCmd = std::prev(cmdHistory.end());
            });
        }
    }

}

namespace ramiel {

    void termInit() {
        assert(!isRunning);
        isRunning = true;
        termThread = std::thread(terminalMain);
    }

    void termExit() {
        assert(isRunning);
        isRunning = false;
        isReading = false;
        isReading.notify_one();
        termThread.join();
    }
        

    void termOpen() {
        assert(isRunning);
        assert(!isOpen);
        isOpen = true;
        prevMenu = TerminalMenu::get().makeCurrent();
        prevInput = InputField::getCurrent();
        InputField::clearCurrent();
        if (isReading) input.makeCurrent();
    }

    void termClose() {
        assert(isRunning);
        assert(isOpen);
        isOpen = false;
        prevMenu ? prevMenu->makeCurrent() : Menu::clearCurrent();
        prevInput ? prevInput->makeCurrent() : InputField::clearCurrent();
    }


    std::string termRead() {
        assert(isRunning);
        assert(!isReading);
        isReading = true;
        addTask([]() {
            if (isOpen) input.makeCurrent();
        });
        isReading.wait(true);
        return std::move(readVal);
    }

    void termWrite(const std::string& msg) {
        assert(isRunning);
        if (isReading) return;
        awaitTask([msg]() {
            size_t linesToBottom = getMaxLine() - text.getLine();
            text.insert(msg);
            if (linesToBottom <= lineStep) text.goToBottom();
            menuRenderNeeded();
        });
    }


    Menu* getTerminal() {
        return &TerminalMenu::get();
    }

}



