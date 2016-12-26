#ifndef WINDOW_H
#define WINDOW_H

#include <irrlicht.h>
#include <cstdint>

class window {
    public:
        window(uint32_t width, uint32_t height, uint32_t depth, bool fullscreen, bool sbuff, bool vsync, IEventReciever* reciever);
        window(uint32_t width, uint32_t height, uint32_t depth);
        ~window();

        void caption(const wchar_t* text);

        IVideoDriver* driver();
        ISceneManager* sceneMgr();
        IGUIEnvironment* guiEnv();

    private:
        IrrlichtDevice *device;
}

#endif
