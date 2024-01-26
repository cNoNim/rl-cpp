cmake_minimum_required(VERSION 3.27)

project(rl-cpp LANGUAGES CXX CUDA)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

include(cmake/configuration.cmake)
include(CMakeDependentOption)

include(cmake/compilation.cmake)
include(cmake/static-analysis.cmake)

list(APPEND CMAKE_PREFIX_PATH ${VIRTUAL_PREFIX}/lib/site-packages/torch)
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS})

add_executable(rl-cpp main.cpp)
target_link_libraries(rl-cpp ${TORCH_LIBRARIES})

if (MSVC)
  file(GLOB TORCH_DLLS ${TORCH_INSTALL_PREFIX}/lib/*.dll)
  add_custom_command(TARGET rl-cpp
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:rl-cpp>)

  set(DEPS_DLLS ${VIRTUAL_PREFIX}/bin/libiomp5md.dll
                ${VIRTUAL_PREFIX}/Library/bin/uv.dll)
  add_custom_command(TARGET rl-cpp
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${DEPS_DLLS}
                    $<TARGET_FILE_DIR:rl-cpp>)
endif (MSVC)
