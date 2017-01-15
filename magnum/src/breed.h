#ifndef BREED_H
#define BREED_H

#include <Magnum/Texture.h>
#include <Magnum/Mesh.h>
#include <Magnum/Trade/ImageData.h>

#include "SillyGame.h"
#include "character.h"

/** @file
 * @brief defines a base type for various characters in the game - both player
 * and not.
 */
namespace SillyGame {

class Character;

class Breed {
    public:
        Breed(Trade::ImageData2D& image);
        Character* newCharacter();

        Texture2D* texture();
        Mesh* mesh();

    private:
        // Only one texture for the image, later this can be an array for standing
        // and moving animation.
        Texture2D texture_;

        // This is always a square.
        Mesh mesh_;

        //TODO: support inheritence when breed stores more than texture & mesh
};
}

#endif
