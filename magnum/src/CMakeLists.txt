find_package(Magnum REQUIRED
    SceneGraph
    Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

set(SOURCES ${SOURCES}
    MyApplication.cpp
    breed.cpp
    character.cpp
    loader.cpp
    )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(SillyGame ${SOURCES})
target_link_libraries(SillyGame
    Magnum::Application
    Magnum::Magnum
    Magnum::SceneGraph)
