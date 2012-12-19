#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <vector>

class shader
{
    public:
    shader(std::vector<char>& vert_shader, std::vector<char>& frag_shader);
    ~shader();
    int add_attribute(std::string attribute);
    
    private:
    void init_shader(std::vector<char>& vert_shader, std::vector<char>& frag_shader);
    GLint load_shader(std::vector<char>& data, GLenum type);
    GLint add_attr(std::string attribute);
    //~ void debug_shader(GLuint &shader_program);

    GLuint program;
    GLint colour_attr, coord_attr;
};

#endif
