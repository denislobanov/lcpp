#ifndef LOADER_H
#define LOADER_H

#include "breed.h"

/**@file
 * @brief load game resources
 */
namespace SillyGame {
class Loader {
    public:
        Loader();
        Breed* newBreed(std::string path);

    private:
        PluginManager::Manager<Trade::AbstractImporter> manager{MAGNUM_PLUGINS_IMPORTER_DIR};
        std::unique_ptr<Trade::AbstractImporter> importer;
};
}

#endif
