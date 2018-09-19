cmake_minimum_required(VERSION 3.5)
project(plugin_loader CXX)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_compile_options(-fPIC)

include_directories(include ${console_bridge_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${Poco_INCLUDE_DIRS})

set(${PROJECT_NAME}_SRCS
    src/plugin_loader.cpp
    src/plugin_loader_core.cpp
    src/meta_object.cpp
    src/multi_library_plugin_loader.cpp
    src/console.cpp
    )
set(${PROJECT_NAME}_HDRS
    include/plugin_loader/plugin_loader.hpp
    include/plugin_loader/plugin_loader_core.hpp
    include/plugin_loader/exceptions.hpp
    include/plugin_loader/meta_object.hpp
    include/plugin_loader/multi_library_plugin_loader.hpp
    include/plugin_loader/register_macro.hpp
    )

if (WIN32)
    set(${PROJECT_NAME}_SRCS
        ${${PROJECT_NAME}_SRCS}
        src/shared_library_WIN32.cpp
        )
endif()

if (UNIX)
    set(${PROJECT_NAME}_SRCS
        ${${PROJECT_NAME}_SRCS}
        src/shared_library_UNIX.cpp
        )
endif (UNIX)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS} ${${PROJECT_NAME}_HDRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${console_bridge_LIBRARIES} dl)
target_compile_definitions(${PROJECT_NAME} PRIVATE "plugin_loader_BUILDING_DLL")

add_subdirectory(test)
