#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glfw.h>

#include "load_file.h"
#include "shader.h"

#define VERT_SHADER_PATH "./shaders/static_draw.vert"
#define FRAG_SHADER_PATH "./shaders/static_draw.frag"

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
        std::cout<<"throwing an exception ;)"<< std::endl;
    
    //possibly figure out rgba from one param?
    res = glfwOpenWindow(width, height, 0, 0, 0, 0, depth, 0, fullscreen);
    if(!res)
        std::cout<<"failed to open window, glfwOpenWindow returned: "<<res<<std::endl;
        
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

int main(void)
{
    std::cout<<"loading shader files"<<std::endl;
    load_file vert_shader(VERT_SHADER_PATH);
    load_file frage_shader(FRAG_SHADER_PATH);
    //~ std::vector<char>& vs = vert_shader.file_data();
    //~ std::vector<char>& fs = frage_shader.file_data();

    std::cout<<"loading shaders"<<std::endl;
    shader le_shader(vert_shader.data, frage_shader.data);
    std::cout<<"done (really?)"<<std::endl;

    return 0;
}
