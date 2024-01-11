cmake_minimum_required(VERSION 3.15...3.22)
project(neat LANGUAGES CXX)

find_package(Python 3.6 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(${PROJECT_NAME} main.cpp)