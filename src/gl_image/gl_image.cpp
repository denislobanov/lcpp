#include <iostream>
#include <GL/glfw>


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

window::window(int width, int height, int depth, int fullscreen)
{
    int res = glfwInit();
    if(!res)
        std::cout << "throwing an exception ;)" << std::endl;
    
    //possibly figure out rgba from one param?
    res = glfwOpenWindow(width, height, 0, 0, 0, 0, depth, 0, fullscreen);
    if(!res)
        std::cout << "failed to open window, glfwOpenWindow returned: " << res << std::endl;
        
    is_fullscreen = fullscreen;
}

window::~window()
{
    glfwCloseWindow();
}

void window::set_pos(int x, int y)
{
    return pos(x, y);
}

void window::set_size(int with, int height)
{
    size(width, height);
}

void window::size(int with, int height)
{
    glfwSetWindowSize(width, height);
}

void window::pos(int x, int y)
{
    glfwSetWindowPos(x, y);
}

void window::toggle_fullscreen()
{
    std::cout << "not yet implemented" << std::endl;
}

