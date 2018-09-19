cmake_minimum_required(VERSION 3.5)

include_directories(../include)

add_library(${PROJECT_NAME}_TestPlugins SHARED plugins.cpp)
target_link_libraries(${PROJECT_NAME}_TestPlugins ${PROJECT_NAME})



add_executable(${PROJECT_NAME}_Test utest.cpp)
target_link_libraries(${PROJECT_NAME}_Test ${PROJECT_NAME} ${PROJECT_NAME}_TestPlugins)
