#include <iostream>
#include <vector>
#include <string>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <glibmm/ustring.h>

#include "parser.hpp"

//Local defines
#define DEBUG 2

#if (defined(DEBUG))&&(DEBUG > 2)
#include <fstream>
#endif

#if defined(DEBUG)
    #define dbg std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #if DEBUG > 1
        #define dbg_2 std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #else
        #define dbg_2 0 && std::cout
    #endif
#else
    #define dbg 0 && std::cout
    #define dbg_2 0 && std::cout
#endif

parser::parser(std::vector<struct parse_param_s>& parse_param)
{
    params = parse_param;

    //hardwired config
    hopts = HTML_PARSE_RECOVER|HTML_PARSE_NOERROR|HTML_PARSE_NOWARNING|HTML_PARSE_NOBLANKS;
}

parser::~parser(void)
{
    // ?
}


}

void parser::parse(Glib::ustring url)
{
    doc = htmlReadFile(url, 0, hopts);
    if(doc) {
        //succesfuly parsed

    } else {
        //raise exception
        std::cerr<<"Failed to succesfully parse "<<url<<std::endl;
    }
}
