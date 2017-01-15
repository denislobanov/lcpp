#include <Magnum/DefaultFramebuffer.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>

#include "SillyGame.h"
#include "loader.h"

namespace SillyGame {
class MyApplication: public Platform::Application {
    public:
        explicit MyApplication(const Arguments& arguments);

    private:
        void drawEvent() override;

        Scene2D _scene;
        Object2D* _cameraObject;
        SceneGraph::Camera2D* _camera;
        SceneGraph::DrawableGroup2D _drawables;
};

MyApplication::MyApplication(const Arguments& arguments): Platform::Application{arguments} {
    /* Configure camera */
    _cameraObject = new Object2D{&_scene};
    _camera = new SceneGraph::Camera2D{*_cameraObject};
    _camera->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
        .setProjectionMatrix(Matrix3::projection({4.0f/3.0f, 1.0f}))
        .setViewport(defaultFramebuffer.viewport().size());

    /* TODO: Prepare your objects here and add them to the scene */

    // loader
    Loader* loader = new Loader();
}

void MyApplication::drawEvent() {
    defaultFramebuffer.clear(FramebufferClear::Color);

    _camera->draw(_drawables);

    swapBuffers();
}

}

MAGNUM_APPLICATION_MAIN(SillyGame::MyApplication)
