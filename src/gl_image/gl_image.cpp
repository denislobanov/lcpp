#include <iostream>
#include <vector>
#include <cstdint>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glfw.h>

#include "load_file.h"
#include "shader.h"
#include "window.h"

#define VERT_SHADER_PATH "./shaders/static_draw.vert"
#define FRAG_SHADER_PATH "./shaders/static_draw.frag"

class square
{
    public:
    square();
    ~square();
    void draw();
    void advance_colour(uint64_t time_elapsed);
    

    private:
    struct cvertex_2d {
        std::vector<GLfloat> pos_2d;
        std::vector<GLfloat> col_rgb;
    };
    
    void init_resources();
    void destroy();

    GLuint vbo;
    shader *le_shader;
};

square::square(void)
{
    std::cout<<"square::square -loading shader files"<<std::endl;
    load_file* vert_shader = new load_file(VERT_SHADER_PATH);
    load_file* frag_shader = new load_file(FRAG_SHADER_PATH);

    std::cout<<"square::square -loading shaders"<<std::endl;
    le_shader = new shader(vert_shader->data, frag_shader->data);

    std::cout<<"square::square -initialising internal resources"<<std::endl;
    init_resources();
}

square::~square()
{
    std::cout<<"square::~square -freeing internal resources"<<std::endl;
    destroy();
    delete vert_shader;
    delete frag_shader;
    delete le_shader;
}

void square::init_resources(void)
{
    struct cvertex_2d attributes[] = {
        {{ -1.0,    1.0 }, { 0.0, 1.0, 0.0 }},
        {{  1.0,    1.0 }, { 0.0, 0.0, 1.0 }},
        {{  1.0,   -1.0 }, { 1.0, 0.0, 0.0 }},
        {{ -1.0,   -1.0 }, { 0.74, 0.4, 0.0 }}
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(attributes), attributes, GL_DYNAMIC_DRAW);
}

void square::destroy(void)
{
    glDeleteBuffers(1, &vbo);
}

void square::advance_colour(uint64_t time_elapsed)
{
    //do something cool
}


int main(void)
{
    std::cout<<"creating window"<<std::endl;
    window* le_window = new window(200, 200, 16, 0);
    
    
    std::cout<<"done (really?)"<<std::endl;

    std::cout<<"cleaning up"<<std::endl;

    delete le_window;
    return 0;
}
