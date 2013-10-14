#include <iostream>
#include <vector>
#include <string>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <glibmm/ustring.h>
#include <glib.h>

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

void parser::save_nodes(struct parse_param_s& param)
{
    struct data_node_s data_entry;
    xmlNodeSetPtr node_set = tags->nodesetval;
    dbg_2<<"saving nodes\n";

    for(int i = 0; i < node_set->nodeNr; ++i) {
        //tag_name
        data_entry.tag_name = reinterpret_cast<const char*>(node_set->nodeTab[i]->name);

        //tag_data
        xmlChar *tmp = xmlNodeListGetString(doc, node_set->nodeTab[i]->children, 1);
        if(tmp > 0)
            data_entry.tag_data = reinterpret_cast<const char*>(tmp);
        xmlFree(tmp);

        //attr_data
        if(!param.attr.empty()) {
            tmp = xmlGetProp(node_set->nodeTab[i], reinterpret_cast<const xmlChar*>(param.attr.c_str()));
            data_entry.attr_data = reinterpret_cast<const char *>(tmp);
            xmlFree(tmp);
        }

        dbg_2<<"tag name ["<<data_entry.tag_name<<"] tag data ["<<data_entry.tag_data<<"] attr_data ["<<data_entry.attr_data<<"]\n";
    }

    data.push_back(data_entry);
}

void parser::parse(Glib::ustring url)
{
    doc = htmlReadFile(url.c_str(), 0, hopts);
    if(doc) {
        //succesfuly parsed
        dbg<<"parsed doc\n";
        xmlXPathContextPtr xpath_ctxt = xmlXPathNewContext(doc);
        if(xpath_ctxt) {
            dbg_2<<"created xpath context, processing tags\n";

            //parse xpath tags
            for(auto& param: params) {
                dbg_2<<"generating xpath\n";
                Glib::ustring xpath = "//" + param.tag;
                if(param.attr.size() > 0)
                    xpath += "[@" + param.attr + "]";

                dbg_2<<"matching xpath ["<<xpath<<"]"<<std::endl;
                tags = xmlXPathEvalExpression(reinterpret_cast<const xmlChar*>(xpath.c_str()), xpath_ctxt);
                dbg_2<<"evaluated expression\n";

                if(!xmlXPathNodeSetIsEmpty(tags->nodesetval)) {
                    dbg<<"got nodes\n";
                    save_nodes(param);
                }
                xmlXPathFreeObject(tags);
            }
            xmlXPathFreeContext(xpath_ctxt);
        } else {
            //FIXME: raise exception
            std::cerr<<"Failed to create xpath_ctxt\n";
        }
    } else {
        //raise exception
        std::cerr<<"Failed to succesfully parse "<<url<<std::endl;
    }
    xmlFreeDoc(doc);
    xmlCleanupParser();
}
