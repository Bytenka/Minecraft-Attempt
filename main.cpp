#include "application/Application.h"

int main(int argc, char *argv[])
{
    tk::Application &appli = tk::Application::getInstance();
    unsigned mainWindow = appli.createWindow("Yaaaaa", 1280, 720);
    appli.setWindowAsMain(mainWindow);
    appli.setWindowIcon(mainWindow, "resources/icon.png");

    appli.startLoop();
    return 0;
}