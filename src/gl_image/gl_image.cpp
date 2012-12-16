#include <iostream>
#include <GL/glfw>

#define VERT_SHADER_PATH "./shaders/static_draw.vert"
#define FRAG_SHADER_PATH "./shaders/static_draw.frag"

class shader
{
    public:
    shader::shader(std::string &vert_shader, std::string &frag_shader);
    shader::~shader();
    int add_attribute(std::string &attribute);
    
    private:
    void init_shader(std::string &vert_shader, std::string &frag_shader);
    const char* load_shader_file(std::string filename);
    GLint load_shader(const char* data, GLenum type);
    GLint add_attr(std::string &attribute);
    void debug_shader(GLuint &shader_program);

    GLuint program;
    GLint colour_attr, coord_attr;
}

shader::shader(std::string &vert_shader, std::string &frag_shader)
{
    init_shader(vert_shader, frag_shader);
}

int shader::add_attribute(std::string &attribute)
{
    add_attr(attribute);
}

void shader::init_shader(std::string &vert_shader, std::string &frag_shader)
{
    GLuint vs, fs;

#if 0
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
    GLuint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(!status) {
        std::cerr<<"glLinkProgram: "<<endl;
        debug_shader(program);
        std:cerr<<std::endl<<"program will NOT work"<<std::endl;
    }
#endif
}

#if 0
//FIXME not cpp
void shader::debug_shader(GLuint &shader_program)
{
    GLint log_length = 0;
    
    if(glIsShader(object))
        glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else if(glIsProgram(object))
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
    else {
        std::cerr<<"debug_shader: Not a shader or a program"<<std::endl;
        return;
    }

    char *log = new char[log_length];

    if(glIsShader(object))
        glGetShaderInfoLog(object, log_length, NULL, log);
        
    else if(glIsProgram(object))
        glGetProgramInfoLog(object, log_length, NULL, log);

    std:cerr<log<<endl;
    delete[] log;
}
#endif

const char* shader::load_shader_file(std::string filename)
{
    
}

class window
{
    public:
    window(int width, int height, int depth, int fullscreen);
    ~window();
    void set_size(int with, int height);
    void set_pos(int x, int y);
    void toggle_fullscreen();
    
    private:
    void size(int width, int height);
    void pos(int x, int y);
    
    int is_fullscreen;
};

window::window(int width, int height, int depth, int fullscreen)
{
    int res = glfwInit();
    if(!res)
        std::cout<<"throwing an exception ;)"<< std::endl;
    
    //possibly figure out rgba from one param?
    res = glfwOpenWindow(width, height, 0, 0, 0, 0, depth, 0, fullscreen);
    if(!res)
        std::cout<<"failed to open window, glfwOpenWindow returned: "<<res<<std::endl;
        
    is_fullscreen = fullscreen;
}

window::~window()
{
    glfwCloseWindow();
}

void window::set_pos(int x, int y)
{
    return pos(x, y);
}

void window::set_size(int with, int height)
{
    size(width, height);
}

void window::size(int with, int height)
{
    glfwSetWindowSize(width, height);
}

void window::pos(int x, int y)
{
    glfwSetWindowPos(x, y);
}

void window::toggle_fullscreen()
{
    std::cout<<"not yet implemented"<<std::endl;
}

int main(void)
{
    std::cout<<"loading shader"<<std::endl;
    shader le_shader = new shader(VERT_SHADER_PATH, FRAG_SHADER_PATH);
    std::cout<<"done (really?)"<<endl;

    return 0;
}
