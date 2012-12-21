#ifndef LOAD_FILE_H
#define LOAD_FILE_H

#include <iostream>
#include <vector>

class load_file
{
    public:
    load_file(std::string filename);
    virtual ~load_file(void);
    std::vector<char> data;

    private:
    void buffer_file_data(std::string filename);
};

#endif
