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
    explicit square(std::string vert_shader_path, std::string frag_shader_path);
    ~square();
    void draw(uint64_t time_elapsed);
    

    private:
    struct cvertex_2d {
        GLfloat pos_2d[2];
        GLfloat col_rgb[3];
    };
    
    void init_resources();
    void destroy();

    GLuint vbo;
    GLint colour_attr, coord_attr;

    protected:
    shader *le_shader;
};

#endif
