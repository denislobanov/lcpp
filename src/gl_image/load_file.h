#ifndef LOAD_FILE_H
#define LOAD_FILE_H

#include <iostream>
#include <vector>

class load_file
{
    public:
    load_file(std::string filename);
    ~load_file();
    std::vector<char>& file_data(void);
    std::vector<char> data;

    private:
    void buffer_file_data(std::string filename);
};

#endif
