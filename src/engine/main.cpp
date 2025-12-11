#include <ramiel/graphics.h>
#include <ramiel/physics.h>

#include "command.h"
#include "window.h"
#include "graphics.h"
#include "physics.h"
#include "task.h"
#include "menu.h"
#include "menu/camera.h"
#include "menu/terminal.h"
using namespace ramiel;


int main(int argc, char* argv[]) {
    if (argc >= 2)
        runCommand(std::string("file load ") + argv[1]);

    initWindow();
    termInit();
    getCameraMenu()->makeCurrent();

    while (windowGood()) {
        setDtime();

        execTasks();

        Menu::currentControls();

        simStep();
        render();

        pollWindowMessages();
    }

    termExit();
    destroyWindow();
    return 0;
}

