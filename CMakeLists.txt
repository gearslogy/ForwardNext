cmake_minimum_required(VERSION 3.20)
project(node_and_items)

include_directories(D:/plugin_dev/libs/GLM_include)
set(CMAKE_CXX_STANDARD 23)

## ---------------------------- Boost include --------------------------------------
# https://cmake.org/cmake/help/v3.15/module/FindBoost.html
set(BOOST_ROOT D:/plugin_dev/libs/boost_1_74_0_b1_rc1)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
    MESSAGE( STATUS "Boost_LIBRARY_DIRS = ${Boost_LIBRARY_DIRS}.")

    link_directories(${Boost_LIBRARY_DIRS})
endif()



file(GLOB CORE_SRC
        core/*.hpp
        core/*.cpp
        core/*.h
        )
set(CORE_INCLUDE ${CMAKE_SOURCE_DIR}/core)
add_subdirectory(core_unit_test)
link_libraries(${Boost_LIBRARIES})