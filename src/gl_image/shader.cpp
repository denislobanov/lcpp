#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glfw.h>

#include "shader.h"

shader::shader(std::vector<char>& vert_shader, std::vector<char>& frag_shader)
{
    init_shader(vert_shader, frag_shader);
}

shader::~shader(void)
{
    glDeleteProgram(program);
}

int shader::add_attribute(std::string attribute)
{
    return add_attr(attribute);
}

GLint shader::add_attr(std::string attribute)
{
    GLint ret = glGetAttribLocation(program, attribute.c_str());
    if(ret <= 0)
        std::cerr<<"shader::add_attr failed to bind attributes "<<attribute<<std::endl;

    return ret;
}

void shader::init_shader(std::vector<char>& vert_shader, std::vector<char>& frag_shader)
{
    GLuint vs, fs;
    
    vs = load_shader(vert_shader, GL_VERTEX_SHADER);
    fs = load_shader(frag_shader, GL_FRAGMENT_SHADER);

    //TODO throw a real exception
    if((vs == 0)||(fs == 0))
        std::cout<<"failed to load shaders. its all buggered"<<std::endl;

    program = glCreateProgram();

    //attach shader
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    //check if link worked
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(!status) {
        std::cerr<<"glLinkProgram: "<<std::endl;
        debug_shader(program);
        std::cerr<<std::endl<<"program will NOT work"<<std::endl;
    }
}

//FIXME not cpp
void shader::debug_shader(GLuint &shader_program)
{
    GLint log_length = 0;
    
    if(glIsShader(shader_program))
        glGetShaderiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
    else if(glIsProgram(shader_program))
        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);
    else {
        std::cerr<<"debug_shader: Not a shader or a program"<<std::endl;
        return;
    }

    char *log = new char[log_length];

    if(glIsShader(shader_program))
        glGetShaderInfoLog(shader_program, log_length, NULL, log);
        
    else if(glIsProgram(shader_program))
        glGetProgramInfoLog(shader_program, log_length, NULL, log);

    std::cerr<<log<<std::endl;
    delete[] log;
}

GLint shader::load_shader(std::vector<char>& data, GLenum type)
{
    GLint status;
    GLuint result;

    data.push_back(0);
    
    //give it to gl
    result = glCreateShader(type);
    const char* data_ptr = &data[0];
    glShaderSource(result, 1, &data_ptr, NULL);

    //compile
    glCompileShader(result);
    status = GL_FALSE;
    
    glGetShaderiv(result, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        std::cerr<<"Compilation failed"<<std::endl;
        debug_shader(result);
        glDeleteShader(result);
        return 0;
    }

    return result;
}

void shader::void draw(GLenum mode, GLint first, GLsizei count)
{
    glUseProgram(program);
    glDrawArrays(mode, first, count);
}
