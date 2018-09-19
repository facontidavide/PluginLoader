cmake_minimum_required(VERSION 3.5)
project(plugin_loader CXX)

add_definitions( -std=c++11 -Wall -Wextra -Wpedantic -fPIC)

include_directories(include )

set(plugin_loader_SRCS
    src/plugin_loader.cpp
    src/plugin_loader_core.cpp
    src/meta_object.cpp
    src/multi_library_plugin_loader.cpp
    src/console.cpp
    src/shared_library_UNIX.cpp
    )

add_library(plugin_loader STATIC ${plugin_loader_SRCS})
target_link_libraries(plugin_loader  dl)

add_subdirectory(example)
