#include <iostream>
#include <vector>
#include <cstdint>
#include <GL/glew.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cube.h"
#include "shader.h"
#include "load_file.h"

cube::cube(std::string vert_shader_path, std::string frag_shader_path):square(vert_shader_path, frag_shader_path)
{
    init_resources();
}

cube::~cube(void)
{

}

void cube::init_resources(void)
{

}
