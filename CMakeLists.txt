cmake_minimum_required(VERSION 3.15...3.22)
project(neat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(/usr/local/include)

find_package(Python 3.6 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(${PROJECT_NAME} MODULE module.cpp lib/config.cpp lib/activation_functions.cpp  lib/math_utils.cpp lib/population.cpp lib/species.cpp lib/genome.cpp lib/connection_gene.cpp lib/connection_history.cpp lib/node.cpp)