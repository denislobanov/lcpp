#ifndef SillyGame_h
#define SillyGame_h

#include <Magnum/Magnum.h>
#include <Magnum/SceneGraph/SceneGraph.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/TranslationTransformation.h>

namespace SillyGame {
using namespace Magnum;

typedef SceneGraph::Object<SceneGraph::TranslationTransformation2D> Object2D;
typedef SceneGraph::Scene<SceneGraph::TranslationTransformation2D> Scene2D;
}
#endif
