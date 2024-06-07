cmake_minimum_required(VERSION 3.15)
project(neat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directory path
include_directories(/usr/local/include)

# Set the path to pybind11
set(pybind11_DIR /usr/local/lib/python3.9/site-packages/pybind11/share/cmake/pybind11)
find_package(Python3 3.6 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

# Add the pybind11 module
pybind11_add_module(${PROJECT_NAME} MODULE
    module.cpp
    lib/config.cpp
    lib/activation_functions.cpp
    lib/math_utils.cpp
    lib/population.cpp
    lib/species.cpp
    lib/genome.cpp
    lib/connection_gene.cpp
    lib/connection_history.cpp
    lib/node.cpp
)
