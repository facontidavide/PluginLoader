cmake_minimum_required(VERSION 3.5)
project(plugin_loader CXX)

add_definitions( -std=c++11 -Wall -Wextra -Wpedantic -fPIC)

include_directories(include )

set(plugin_loader_SRCS
    src/Poco/Manifest.cpp
    src/Poco/Exception.cpp
    src/Poco/SharedLibrary.cpp
    )

add_library(plugin_loader STATIC ${plugin_loader_SRCS})
target_link_libraries(plugin_loader  dl)

add_subdirectory(example)

