cmake_minimum_required(VERSION 3.5)

add_library(Plugins SHARED plugins.cpp)
target_link_libraries(Plugins plugin_loader)

add_executable(TestPlugins utest.cpp)
target_link_libraries(TestPlugins plugin_loader)
