#include <iostream>
#include <fstream>
#include <sstream>

#include "json.hpp"

using json = nlohmann::json;
using std::cout;
using std::endl;

int main(void) {
    //load json data
    std::ifstream f("test.json");
    if(f) {
        json j;

        // read file
        f >> j;
        
        // print to screen
        cout << j.at("a") << endl;

        // close
        f.close();
    }

    return 0;
}
