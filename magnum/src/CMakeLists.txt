find_package(Magnum REQUIRED
    SceneGraph
    Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

include_directories(../lib)

set(SillyGame_SRC
    Application.cpp
    )

add_executable(SillyGame Application.cpp)
target_link_libraries(SillyGame
    Magnum::Application
    Magnum::Magnum
    Magnum::SceneGraph)
