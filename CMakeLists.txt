cmake_minimum_required(VERSION 3.27)

project(rl-cpp LANGUAGES C CXX CUDA)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

include(cmake/mamba.cmake)
include(cmake/configuration.cmake)
include(CMakeDependentOption)

include(cmake/compilation.cmake)
include(cmake/static-analysis.cmake)

add_subdirectory(extensions)
add_subdirectory(echo)
add_subdirectory(sfml-app)
# add_subdirectory(rl-cpp)
