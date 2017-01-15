#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/TextureData.h>

#include "breed.h"
#include "loader.h"
#include "config.h"
#include "../lib/json.hpp"

using json = nlohmann::json;

namespace SillyGame {

Loader::Loader() {
    PluginManager::Manager<Trade::AbstractImporter> manager{MAGNUM_PLUGINS_IMPORTER_DIR};
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
