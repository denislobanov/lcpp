#ifndef BREED_H
#define BREED_H

/** @file
 * @brief defines a base type for various characters in the game - both player
 * and not.
 */

#include <Magnum/SceneGraph/Drawable.h>

#include "SillyGame.h"
#include "character.h"

namespace SillyGame {
class Breed {
    public:
        Breed(Texture2D& tex);
        inline Character* newCharacter() { return new Character(*this); }

        Texture2D* texture();
        Mesh* mesh();

    private:
        // Only one texture for the image, later this can be an array for standing
        // and moving animation.
        Texture2D texture;

        // This is always a square.
        Mesh mesh;

        //TODO: support inheritence when breed stores more than texture & mesh
};
}

#endif
