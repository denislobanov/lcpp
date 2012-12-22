#include <iostream>
#include <unistd.h>

#include "load_file.h"
#include "shader.h"
#include "window.h"
#include "square.h"

#define VERT_SHADER_PATH "./shaders/static_draw.vert"
#define FRAG_SHADER_PATH "./shaders/static_draw.frag"


int main(void)
{
    std::cout<<"creating window"<<std::endl;
    window* le_window = new window(200, 200, 8);
    
    std::cout<<"creating square"<<std::endl;
    square my_square(VERT_SHADER_PATH, FRAG_SHADER_PATH);
    int i;
    for(i=0;i<10;i++) {
        my_square.draw();
        sleep(1);
    }

    std::cout<<"cleaning up"<<std::endl;
    delete le_window;
    return 0;
}
