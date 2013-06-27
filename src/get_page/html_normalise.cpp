#include <iostream>
#include <vector>
#include <sstream>

#include <tidy.h>
#include <buffio.h>
#include "html_normalise.hpp"

//Local defines
#if defined(DEBUG)
    #define dbg std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #if DEBUG > 1
        #define dbg_1 std::cout<<__FILE__<<"("<<__LINE__<<"): "
    #else
        #define dbg_1 0 && std::cout
    #endif
#else
    #define dbg 0 && std::cout
    #define dbg_1 0 && std::cout
#endif

html_normalise::html_normalise(void)
{
}

html_normalise::~html_normalise(void)
{
}

int html_normalise::normalise(std::string& data)
{
    //can this be done in the constructor?
    TidyBuffer out_buf = {0};
    TidyBuffer err_buf = {0};
    TidyDoc doc = tidyCreate();

    //convert to xhtml. xml and html are valid options too. try?
    dbg<<"tidyOptSetBool\n";
    tidyOptSetBool(doc, TidyXhtmlOut, yes);
    //connect error buffer
    dbg<<"tidySetErrorBuffer\n";
    tidySetErrorBuffer(doc, &err_buf);
    
    //parse data
    dbg<<"parsing\n";
    int ret = tidyParseString(doc, data.c_str());

    //normalise
    if(ret >= 0) {
        dbg<<"clean and repair\n";
        ret = tidyCleanAndRepair(doc);
    }

    //check for errors, to force output if any were encountered
    if(ret >= 0)
        ret = tidyRunDiagnostics(doc);
    if(ret > 1)
        ret = (tidyOptSetBool(doc, TidyForceOutput, yes)? ret : -1);

    //save data
    if(ret >= 0) {
        dbg<<"saving data\n";
        std::stringstream ss;
        ss<<out_buf.bp;
        std::string temp = ss.str();
        data = temp;
    }

    //cleanup
    tidyBufFree(&out_buf);
    tidyBufFree(&err_buf);
    tidyRelease(doc);
    return ret;
}
