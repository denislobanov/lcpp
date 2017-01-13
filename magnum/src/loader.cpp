#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "breed.h"
#include "loader.h"
#include "../lib/json.hpp"

using json = nlohmann::json;

namespace SillyGame {

Loader::Loader() {
    importer = manager.loadAndInstantiate("AnyImageImporter");
}

Breed* Loader::newBreed(std::string path) {
    std::ifstream file(path);
    if(file) {
        json j;
        file >> j;

        // Load texture from config
        //
        //j.at("texture")
    }

    return 0;
}
}
