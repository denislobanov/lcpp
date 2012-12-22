#include <iostream>
#include <GL/glfw.h>

#include "window.h"

window::window(int width, int height, int depth)
{
    int res = glfwInit();
    if(!res)
        std::cout<<"throwing an exception ;)"<< std::endl;
    
    //possibly figure out rgba from one param?
    res = glfwOpenWindow(width, height, 0, 0, 0, 0, depth, 0, GLFW_WINDOW);
    if(!res)
        std::cout<<"failed to open window, glfwOpenWindow returned: "<<res<<std::endl;
}

window::~window()
{
    glfwCloseWindow();
}

void window::set_pos(int x, int y)
{
    return pos(x, y);
}

void window::set_size(int width, int height)
{
    size(width, height);
}

void window::size(int width, int height)
{
    glfwSetWindowSize(width, height);
}

void window::pos(int x, int y)
{
    glfwSetWindowPos(x, y);
}

void window::toggle_fullscreen()
{
    std::cout<<"not yet implemented"<<std::endl;
}
