#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <vector>
#include <cstdint>
#include <GL/gl.h>

#include "shader.h"
#include "square.h"

class cube: public square
{
    public:
    cube(std::string vert_shader_path, std::string frag_shader_path);
    ~cube();
    void draw(uint64_t time_elapsed);

    private:
    void init_resources();

    GLuint vbo_vert, vbo_colour;
    GLuint ibo_elements;
    GLint attr_coord, attr_colour;
    GLint uniform_mvp;
};


#endif
