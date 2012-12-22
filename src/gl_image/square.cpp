#include <iostream>
#include <vector>
#include <cstdint>
#include <GL/glew.h>
#include <GL/gl.h>

#include "square.h"
#include "shader.h"
#include "load_file.h"

square::square(std::string vert_shader_path, std::string frag_shader_path)
{
    std::cout<<"square::square -loading shader files"<<std::endl;
    load_file vert_shader(vert_shader_path);
    load_file frag_shader(frag_shader_path);

    std::cout<<"square::square -loading shaders"<<std::endl;
    le_shader = new shader(vert_shader.data, frag_shader.data);

    std::cout<<"square::square -initialising internal resources"<<std::endl;
    init_resources();
}

square::~square()
{
    std::cout<<"square::~square -freeing internal resources"<<std::endl;
    destroy();
    delete le_shader;
}

void square::init_resources(void)
{
    std::cout<<"square::init_resources -adding attributes"<<std::endl;
    coord_attr = le_shader->add_attribute("coord_2d");
    if(coord_attr) {
        colour_attr = le_shader->add_attribute("coord_colour");
        if(colour_attr)
            std::cout<<"square::init_resourcesdone"<<std::endl;
        else
            std::cerr<<"square::init_resourcesfailed to add colour_attr"<<std::endl;
    } else
    {
        std::cerr<<"square::init_resourcesfailed to add coord_attr"<<std::endl;
    }
        
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

void square::draw(void)
{
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClear(GL_COLOR_BUFFER_BIT);
    glEnableVertexAttribArray(coord_attr);
    glEnableVertexAttribArray(colour_attr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //describe vert array
    glVertexAttribPointer(
        coord_attr,
        2, 
        GL_FLOAT, 
        GL_FALSE,
        sizeof(struct cvertex_2d),
        0 
    );
    
    glVertexAttribPointer(
        colour_attr,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(struct cvertex_2d),
        (GLvoid*)offsetof(struct cvertex_2d, col_rgb)
    );
}
