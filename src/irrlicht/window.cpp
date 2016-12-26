#include "window.h"

window::window(uint32_t width, uint32_t height, uint32_t depth, bool fullscreen,
               bool sbuff, bool vsync, IEventReciever* reciever) {
    device = createDevice(irr::video::EDT_OPENGL,
                         irr:core::dimension2d<uint32_t>(width, height),
                         depth,
                         fullscreen,
                         sbuff,
                         vsync,
                         reciever);
}

window::window(uint32_t width, uint32_t height, uint32_t depth) {
    window(width, height, depth, false, false, false, 0);
}

window::~window() {
    device->drop();
}

void window::caption(const wchar_t* text) {
    device->setWindowCaption(text);
}

IVideoDriver window::driver() {
    return device->getVideoDriver();
}

ISceneManager window::sceneMgr() {
    return device->getSceneManager();
}

IGUIEnvironment window::guiEnv() {
    return device->getGUIEnvironment();
}

