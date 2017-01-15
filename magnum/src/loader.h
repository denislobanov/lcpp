#ifndef LOADER_H
#define LOADER_H

#include <Magnum/Trade/ImageData.h>
#include <Magnum/Trade/AbstractImporter.h>

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
        std::unique_ptr<Trade::AbstractImporter> importer;
};
}
 
#endif
