#include <unistd.h>
#include <irrlicht/irrlicht.h>

using namespace irr;

IrrlichtDevice* createWindow(u32 width, u32 height, u32 depth) {
    return createDevice(video::EDT_OPENGL,
                        core::dimension2d<u32>(width, height),
                        depth,
                        false, false, false, 0);
}

int main(void) {
    // Create window
    IrrlichtDevice *device = createWindow(240, 240, 32);
    device->setWindowCaption(L"bla bla bla");

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* mgr = device->getSceneManager();
    gui::IGUIEnvironment* guiEnv = device->getGUIEnvironment();

    video::ITexture* texture = driver->getTexture("~/images/wallpaper/bikes/2010_Triumph_Sprint_S_T_exhaust_1680x1260.jpg");


    while(device->run() && driver) {
        driver->beginScene(true, true, video::SColor(256, 65, 86, 139));

        driver->draw2DImage(texture, core::position2d<s32>(0,0));
        mgr->drawAll();
        guiEnv->drawAll();

        driver->endScene();
    }

    device->drop();
    return 0;
}

