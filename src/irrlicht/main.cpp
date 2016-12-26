#include <unistd.h>

#include "window.h"

int main(void) {
    // Create window
    Window* w = new Window(240,240,16);
    w->caption("bla bla bla");

    IVideoDriver* driver = w->driver();
    ISceneManager* mgr = w->sceneMgr();
    IGUIEnvironment guiEnv = w->guiEnv();

    driver->beginScene(true, true, sColor(65, 86, 139, 140));
    mgr->drawAll();
    guiEnv->drawAll();

    sleep(10);

    driver->endScene();

    return 0;
}




