find_package(Magnum REQUIRED
    SceneGraph
    Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(SOURCES ${SOURCES}
    MyApplication.cpp
    loader.cpp
    )

configure_file("config.h.in" "config.h")

add_executable(SillyGame ${SOURCES})
target_link_libraries(SillyGame
    Magnum::Application
    Magnum::Magnum
    Magnum::SceneGraph)
