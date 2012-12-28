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

    private:
    void init_resources();
};


#endif
