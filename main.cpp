#include "application/Application.h"
#include "application/RenderWindow.h"
#include "graphics/Image.h"

int main(int argc, char *argv[])
{
    tk::Application &appli = tk::Application::getInstance();
    unsigned mainWindow = appli.createRenderWindow("Yaaaaa", 1280, 720);
    appli.setWindowAsMain(mainWindow);

    // Setting things up for the window
    {
        using namespace tk;

        RenderWindow *w = dynamic_cast<RenderWindow *>(appli.getInternalWindow(mainWindow));
        w->setIcon(Image("resources/icon.png"));
        w->setUseMouseForCamera(true);
    }

    appli.startLoop();
    return 0;
}