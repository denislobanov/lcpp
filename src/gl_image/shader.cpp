#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glfw.h>

#include "shader.h"

shader::shader(std::vector<char>& vert_shader, std::vector<char>& frag_shader)
{
    //we need to initialise GLEW first
    std::cout<<"shader::shader -initialising GLEW"<<std::endl;
    GLenum glew_status = glewInit();
    if(glew_status == GLEW_OK) {
        if(GLEW_VERSION_2_0) {
            //all okay, go ahead
            init_shader(vert_shader, frag_shader);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            std::cout<<"shader::shader -initialised"<<std::endl;
        } else {
            std::cerr<<"shader::shader -Your graphics card does not support OpenGL 2.0"<<std::endl;
        }
    } else {
        std::cerr<<"shader::shader -Error:"<<glewGetErrorString(glew_status)<<std::endl;
    }
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
    std::cout<<"shader::init_shader -loading shaders"<<std::endl;
    GLuint vs, fs;
    vs = load_shader(vert_shader, GL_VERTEX_SHADER);
    fs = load_shader(frag_shader, GL_FRAGMENT_SHADER);

    //TODO throw a real exception?
    if((vs > 0)&&(fs > 0)) {
        program = glCreateProgram();

        //attach shader
        std::cout<<"shader::init_shader -attaching shaders"<<std::endl;
        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);

        //check if link worked
        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if(!status) {
            std::cerr<<"shader::init_shader -glLinkProgram: "<<std::endl;
            debug_shader(program);
            std::cerr<<std::endl<<"shader::init_shader -program will NOT work"<<std::endl;
        }
    }
    else {
        std::cout<<"shader::init_shader -failed to load shaders. its all buggered"<<std::endl;
    }
}

//FIXME not cpp
void shader::debug_shader(GLuint &shader_program)
{
    std::cout<<"shader::debug_shader start"<<std::endl;
    
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
    std::cout<<"shader::debug_shader finish"<<std::endl;
}

GLint shader::load_shader(std::vector<char>& data, GLenum type)
{
    GLint status;
    GLuint result;

    data.push_back(0);
    
    //give it to gl
    result = glCreateShader(type);
    const GLchar* data_ptr = &data[0];
    if(*data_ptr == 0) {
        std::cerr<<"shader::load_shader -*data_ptr == NULL"<<std::endl;
        return -1;
    } else {
        std::cout<<"shader::load_shader - *data_ptr valid"<<std::endl;
    }
    
    glShaderSource(result, 1, &data_ptr, 0);

    //compile
    glCompileShader(result);
    status = GL_FALSE;
    
    glGetShaderiv(result, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        std::cerr<<"Compilation failed"<<std::endl;
        debug_shader(result);
        glDeleteShader(result);
        return -1;
    }

    std::cout<<"shader::load_shader -success"<<std::endl;

    return result;
}

void shader::draw(GLenum mode, GLint first, GLsizei count)
{
    glUseProgram(program);
    glDrawArrays(mode, first, count);
}
