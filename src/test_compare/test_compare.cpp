#include <iostream>

int main(void)
{
    std::string data = "test data         ";
    std::string agent_name = "test data";

    size_t res = data.compare(0, data.size(), agent_name);
    std::cout<<"result: "<<res<<std::endl;

    return 0;
}
