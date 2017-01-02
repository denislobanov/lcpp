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

void cube::init_resources(void)
{
    GLfloat cube_vertices[] = {
        // front
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // back
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
    };
    glGenBuffers(1, &vbo_vert);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    GLfloat cube_colours[] = {
        // front colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
        // back colors
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 1.0, 1.0,
    };
    glGenBuffers(1, &vbo_colour);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colour);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colours), cube_colours, GL_STATIC_DRAW);

    GLushort cube_elements[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // top
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // bottom
        4, 0, 3,
        3, 7, 4,
        // left
        4, 5, 1,
        1, 0, 4,
        // right
        3, 2, 6,
        6, 7, 3,
    };
    glGenBuffers(1, &ibo_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    attr_coord = le_shader->add_attribute("coord_3d");
    if(attr_coord > -1) {
        attr_colour = le_shader->add_attribute("coord_colour");
        if(attr_colour > -1) {
            uniform_mvp = le_shader->add_uniform("mvp");
            if(uniform_mvp > -1)
                std::cout<<"cube::init_resources -done"<<std::endl;
        }
    }

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

cube::~cube(void)
{
    glDeleteBuffers(1, &vbo_vert);
    glDeleteBuffers(1, &vbo_colour);
    glDeleteBuffers(1, &ibo_elements);
}

void cube::draw(uint64_t time_elapsed)
{
    float angle = (time_elapsed/1000.0)*45;  // 45° per second
    glm::mat4 anim = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0,1,0));

    //create transform matricies
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
    glm::mat4 view = glm::lookAt(glm::vec3(0.0,2.0,0.0), glm::vec3(0.0,0.0,-4.0), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 10.0f);

    glm::mat4 mvp = projection*view*model*anim;
    glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

    glEnableVertexAttribArray(attr_colour);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_colour);
    glVertexAttribPointer(
        attr_colour,       // attribute
        3,                 // number of elements per vertex, here (x,y,z)
        GL_FLOAT,          // the type of each element
        GL_FALSE,          // take our values as-is
        0,                 // no extra data between each position
        0                  // offset of first element
    );

    //verticies
    glEnableVertexAttribArray(attr_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
    glVertexAttribPointer(
        attr_coord,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );

    //Push each element in buffer_vertices to the vertex shader
    int size;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    le_shader->draw_elements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    glDisableVertexAttribArray(attr_colour);
    glDisableVertexAttribArray(attr_coord);
}
