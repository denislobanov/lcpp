#include <iostream>

struct test_data_s {
    int a;
    unsigned int b;
};

struct test_data_s* allocate_data(int my_a)
{
    struct test_data_s* temp;

    temp = new test_data_s;
    temp->a = my_a;
    
    return temp;
}

int main(void)
{
    struct test_data_s* my_data;
    int i = 5;
    unsigned int u = 1024;
    
    std::cout<<"allocating new structure, int = "<<i<<std::endl;

    my_data = allocate_data(i);

    if(my_data != 0) {
        my_data->b = u;
        std::cout<<"success! my_data->a = "<<my_data->a<<" my_data->b = "<<my_data->b<<std::endl;

        delete my_data;
    } else {
        std::cout<<"FAILED!"<<std::endl;
    }
    
    return 0;
}
