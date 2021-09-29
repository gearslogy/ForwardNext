cmake_minimum_required(VERSION 3.20)
project(node_and_items)

file(
        GLOB
        SRC
        *.cpp
        *.h
        *.hpp
)
include_directories(${CORE_INCLUDE})
MESSAGE(${CORE_INCLUDE})
add_executable(unit_test_core ${SRC} ${CORE_SRC})