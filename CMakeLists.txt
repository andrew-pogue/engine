cmake_minimum_required (VERSION 3.13.0)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set (CMAKE_CXX_STANDARD 20)

project (Game VERSION 1.0.0 LANGUAGES C CXX)

file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")

add_executable (game ${SOURCES})

target_include_directories (game PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_link_libraries (
    game
    allegro
    allegro_main
    allegro_font
    allegro_ttf
    allegro_primitives
    allegro_color
)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets" SYMBOLIC) 

