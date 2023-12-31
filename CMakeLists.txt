cmake_minimum_required (VERSION 3.13.0)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set (CMAKE_CXX_STANDARD 20)

project (LearnCmake VERSION 1.0.0 LANGUAGES C CXX)

file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
file (GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")

add_executable (hello ${SOURCES})

target_include_directories (hello PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories (hello PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_directories (hello PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib/win64")
# target_link_directories (hello PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/bin")

target_link_libraries (
    hello
    allegro
    allegro_font
    allegro_primitives
)

# target_compile_definitions ("${CMAKE_PROJECT_NAME}" PUBLIC ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")
