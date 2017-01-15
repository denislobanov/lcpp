#ifndef CHARACTER_H
#define CHARACTER_H

#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/Texture.h>

#include "SillyGame.h"
#include "breed.h"

/** @file
 * @brief Base character class extends magnum scene object. Uses Breed class to
 * specialise.
 */
namespace SillyGame {

class Breed;

class Character: public Object2D, public SceneGraph::Drawable2D {
    friend class Breed;

    public:
        void draw(const Matrix3 transformationMatrix, SceneGraph::Camera2D& camera);

    private:
        Character(const Breed& breed);//: texture(breed.texture()), mesh(breed.mesh()) {}

        Vector2i position;
        Texture2D& texture;
        Mesh& mesh;
};
}
#endif
