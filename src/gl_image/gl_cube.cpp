#include <iostream>
#include <unistd.h>
#include <sys/time.h>

#include "load_file.h"
#include "shader.h"
#include "window.h"
#include "cube.h"

#define VERT_SHADER_PATH "./shaders/cube_draw.vert"
#define FRAG_SHADER_PATH "./shaders/cube_draw.frag"
#define SLEEP_USEC 100000


int main(void)
{
    std::cout<<"main -initialising resources"<<std::endl;
    window* le_window = new window(200, 200, 8);
    cube my_cube(VERT_SHADER_PATH, FRAG_SHADER_PATH);
    struct timeval start_time, current_time;
    
    std::cout<<"main -drawing"<<std::endl;
    while(!le_window->exit_key_press()) {
        gettimeofday(&start_time, 0);
        my_cube.draw(start_time.tv_usec);
        le_window->swap_buffers();
        
        //sleep
        gettimeofday(&current_time, 0);
        usleep(SLEEP_USEC-(current_time.tv_usec - start_time.tv_usec));
    }

    std::cout<<"cleaning up"<<std::endl;
    delete le_window;
    return 0;
}
