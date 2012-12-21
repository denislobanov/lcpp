#ifndef WINDOW_H
#define WINDOW_H

#include <GL/gl.h>

class window
{
    public:
    window(int width, int height, int depth, int fullscreen);
    ~window();
    void set_size(int with, int height);
    void set_pos(int x, int y);
    void toggle_fullscreen();
    
    private:
    void size(int width, int height);
    void pos(int x, int y);
    
    int is_fullscreen;
};

#endif
