#include <thread>

#include <ramiel/graphics.h>
#include <ramiel/physics.h>

#include "command.h"
#include "window.h"
#include "graphics.h"
#include "physics.h"
#include "task.h"
#include "menu.h"
#include "menu/camera.h"
using namespace ramiel;


void runCommandLine() {
    std::string command;
    while (true) {
        std::cout << getPath() << "> ";
        std::getline(std::cin, command);
        runCommand(command);
    }
}


int main(int argc, char* argv[]) {
    if (argc >= 2)
        runCommand(std::string("file load ") + argv[1]);

    initWindow();
    std::thread(runCommandLine).detach();
    getCameraMenu()->makeCurrent();

    while (windowGood()) {
        setDtime();

        execTasks();

        Menu::currentControls();

        simStep();
        render();

        pollWindowMessages();
    }

    destroyWindow();
    return 0;
}

