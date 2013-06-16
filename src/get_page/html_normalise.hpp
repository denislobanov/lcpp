#if !defined(NORMALIZE_HTML_H)
#define NORMALIZE_HTML_H

#include <iostream>
#include <vector>

#include <tidy.h>

class html_normalise
{
    public:
    html_normalise(void);
    ~html_normalise(void);

    int normalise(std::string& data);
};

#endif
