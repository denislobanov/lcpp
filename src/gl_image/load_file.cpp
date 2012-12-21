#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>

#include "load_file.h"

load_file::load_file(std::string filename)
{
    buffer_file_data(filename);
}

void load_file::buffer_file_data(std::string filename)
{
    std::ifstream file(filename.c_str());

    if(file) {
        file>>std::noskipws;
        std::copy(std::istream_iterator<char>(file),
                    std::istream_iterator<char>(),
                    std::back_inserter(data));
    }
}
