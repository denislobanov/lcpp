#ifndef WINDOW_H
#define WINDOW_H


class window
{
    public:
    window(int width, int height, int depth);
    ~window();
    void set_size(int with, int height);
    void set_pos(int x, int y);
    void toggle_fullscreen();
    void swap_buffers();
    bool exit_key_press();
    
    private:
    void size(int width, int height);
    void pos(int x, int y);
};

#endif
