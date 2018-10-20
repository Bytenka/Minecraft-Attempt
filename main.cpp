#include "application/Application.h"
#include "application/Window.h"
#include "graphics/Image.h"

int main(int argc, char *argv[])
{
    tk::Application &appli = tk::Application::getInstance();
    unsigned mainWindow = appli.createWindow("Yaaaaa", 1280, 720);
    appli.setWindowAsMain(mainWindow);

    // Setting things up for the window
    {
        using namespace tk;

        Window *w = appli.getInternalWindow(mainWindow);
        w->setIcon(Image("resources/icon.png"));
        w->setUseMouseForCamera(false);
    }

    appli.startLoop();
    return 0;
}