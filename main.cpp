#include "application/Application.h"

int main(int argc, char *argv[])
{
    tk::Application &appli = tk::Application::getInstance();
    appli.createWindow("Yaaaaa", 1280, 720);
    appli.setWindowAsMain("Yaaaaa"); // @TODO Make a UID system for windows
    appli.createWindow("2", 200, 200);
    appli.createWindow("3", 300, 300);

    appli.startLoop();
    return 0;
}