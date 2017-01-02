#include <iostream>
#include <regex>

int main(void)
{
    std::string data = "test data         ";
    std::string agent_name = "test data";

    size_t res = data.compare(0, data.size(), agent_name);
    std::cout<<"result: "<<res<<std::endl;

    std::string test_path = "/random_directory/common_name/other_sub";
    std::string test_common = "*/common_name/";
    std::string test_common_san = "/common_name/";

    //regexp

    std::cout<<"regex matching common.. ";
    if(std::regex_search(test_path.begin(), test_path.end(), std::regex(test_common)))
        std::cout<<"ok"<<std::endl;
    else
        std::cout<<"fail"<<std::endl;

    std::cout<<"string compare common.. ";
    res = test_path.compare(0, test_path.size(), test_common_san, 0, test_common_san.size());
    if((res == 0)||(res == test_common_san.size()))
        std::cout<<"ok"<<std::endl;
    else
        std::cout<<"fail"<<std::endl;
    
    return 0;
}
