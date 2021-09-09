cmake_minimum_required(VERSION 3.20)
project(node_and_items)

set(CMAKE_CXX_STANDARD 23)

add_executable(node_and_items main.cpp PCG_NodePort.cpp PCG_NodePort.h PCG_NodeLink.cpp PCG_NodeLink.h PCG_Types.h PCG_Detail.hpp)
