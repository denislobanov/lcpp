#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <Corrade/PluginManager/Manager.h>
#include <Magnum/Trade/AbstractImporter.h>
#include <Magnum/Trade/ImageData.h>

#include "breed.h"
#include "loader.h"
#include "config.h"
#include "../lib/json.hpp"

using json = nlohmann::json;

namespace SillyGame {

Loader::Loader() {
    PluginManager::Manager<Trade::AbstractImporter> manager{MAGNUM_PLUGINS_IMPORTER_DIR};
    importer = manager.loadAndInstantiate("AnyImageImporter");
    if(!importer) {
        Error() << "Cannot load importer plugin from" << manager.pluginDirectory();
        std::exit(1);
    }
}

Breed* Loader::newBreed(const std::string path) {
    std::ifstream file(path);
    if(file) {
        json conf;
        file >> conf;

        // Load texture
        importer->openFile(conf.at("texture"));
        if(importer->isOpened()) {
            std::optional<Trade::ImageData2D> image = importer->image2D(0);
            CORRADE_INTERNAL_ASSERT(image);

            Breed* breed = new Breed(*image);
            return breed;
        }
    }

    return nullptr;
}
}
