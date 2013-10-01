#include <iostream>
#include <vector>
#include <string>
#include <libxml/HTMLparser.h>
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

void parser::reconfigure(std::vector<struct parse_param_s>& parse_param)
{
    params = parse_param;
}

void parser::get_data(std::vector<struct data_node_s>& copy_data)
{
    dbg<<"data length "<<data.size()<<std::endl;
    copy_data = data;

    dbg<<"copy_data size "<<copy_data.size()<<std::endl;
}

void parser::parse(Glib::ustring url)
{
    doc = htmlReadFile(url.c_str(), 0, hopts);
    if(doc) {
        //succesfuly parsed
        dbg<<"parsed doc\n";
        xpath_ctxt = xmlXPathNewContext(doc);
        if(xpath_ctxt) {
            //parse xpath tags
            dbg<<"created xpath context, processing tags\n";
            for(auto& param: params) {
                dbg_2<<"generating xpath\n";
                Glib::ustring xpath = "//" + param.tag;
                if(param.attr.size() > 0)
                    xpath += "[@" + param.attr + "]";

                dbg_2<<"matching xpath ["<<xpath<<"]"<<std::endl;
                //FIXME: see how libxml++ converts between Glib::ustring
                //and xmlChar*
                tags = xmlXPathEvalExpression((xmlChar *)xpath.c_str(), xpath_ctxt);
                if(tags)
                    dbg<<"found tags!!\n";
            }
        } else {
            //FIXME: raise exception
            std::cerr<<"Failed to create xpath_ctxt\n";
        }
    } else {
        //raise exception
        std::cerr<<"Failed to succesfully parse "<<url<<std::endl;
    }
}
