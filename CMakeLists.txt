cmake_minimum_required(VERSION 2.6)

project(openpsy)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

add_subdirectory(plotter)
add_subdirectory(TestWindow)
add_subdirectory(launcher)
