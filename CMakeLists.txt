cmake_minimum_required(VERSION 3.11)

project(pybind_check)
add_subdirectory(pybind11)
pybind11_add_module(ImagePixelizer pymain.cpp src/ImgManipulator.cpp)