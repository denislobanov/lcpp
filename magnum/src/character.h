#ifndef CHARACTER_H
#define CHARACTER_H

#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/ResourceManager.h>

#include "SillyGame.h"
#include "breed.h"

/** @file
 * @brief Base character class extends magnum scene object. Uses Breed class to
 * specialise.
 */
namespace SillyGame {
class Character: public Object2D,  SceneGraph::Drawable2D {
    friend class Breed;

    public:
        void draw(const Matrix3D transformationMatrix, SceneGraph::Camera<dimensions, T>& camera) override;

    private:
        Character(Breed& breed): texture(breed.texture()), mesh(breed.mesh()) {}

        Vector2i position;
        Texture2D& texture;
        Mesh& mesh;
};
}
#endif
