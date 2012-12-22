#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <GL/gl.h>

#include "shader.h"

class square
{
    public:
    square(std::string vert_shader_path, std::string frag_shader_path);
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
    GLint colour_attr, coord_attr;
    shader *le_shader;
};

#endif
