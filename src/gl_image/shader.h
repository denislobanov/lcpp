#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <vector>
#include <GL/gl.h>

class shader
{
    public:
    shader(std::vector<char>& vert_shader, std::vector<char>& frag_shader);
    ~shader();
    int add_attribute(std::string attribute);
    int add_uniform(std::string uniform);
    void draw_arrays(GLenum mode, GLint first, GLsizei count);
    void draw_elements(GLenum mode, GLsizei count, GLenum type, const GLvoid* indicies);
    
    private:
    void init_shader(std::vector<char>& vert_shader, std::vector<char>& frag_shader);
    GLint load_shader(std::vector<char>& data, GLenum type);
    void debug_shader(GLuint &shader_program);
    bool init_glew();
    void init_gl();

    protected:
    GLuint program;
};

#endif
