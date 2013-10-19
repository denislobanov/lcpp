#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>

#include "parser.hpp"

using std::cout;
using std::endl;

int main(void)
{
    cout<<"loading test page"<<endl;
    std::ifstream file("test_file.html");
    std::string web_page;

    file.seekg(0, std::ios::end);
    //~ web_page.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    //~ web_page.assign((std::istreambuf_iterator<char>(file)),
        //~ std::istreambuf_iterator<char>());

    //html tags to look for
    cout<<"creating parser config"<<endl;
    std::vector<struct parse_param_s> parse_param;
    struct parse_param_s param;

    param.tag = "a";
    param.attr = "href";
    parse_param.push_back(param);

    param.tag = "img";
    param.attr = "src";
    parse_param.push_back(param);

    param.tag = "title";
    param.attr = "";
    parse_param.push_back(param);

    //create parser
    cout<<"initialising parser"<<endl;
    parser test_parser("http://www.xmlsoft.org/", parse_param);

    cout<<"parsing"<<endl;
    //try catch block here
    test_parser.parse();

    //process data
    cout<<"processing data"<<endl;
    int i = 0;
    for(auto& entry: test_parser.data) {
        cout<<"entry "<<i<<endl;
        cout<<"tag name ["<<entry.tag_name<<"]"<<endl;
        cout<<"tag data ["<<entry.tag_data<<"]"<<endl;
        cout<<"tag attr_data ["<<entry.attr_data<<"]"<<endl;
        cout<<endl;
        ++i;
    }

    cout<<"done"<<endl;
    return 0;
}
