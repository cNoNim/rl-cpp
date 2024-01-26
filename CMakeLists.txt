cmake_minimum_required(VERSION 3.27)
project(rl-cpp)

set(CONDA_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/.conda")

list(APPEND CMAKE_PREFIX_PATH "${CONDA_PREFIX}/lib/site-packages/torch")

link_directories("${CONDA_PREFIX}/Library/lib")
set(ENV{MKLROOT} "$ENV{CONDA_PREFIX}/lib")
find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_executable(rl-cpp main.cpp)
set_property(TARGET rl-cpp PROPERTY CXX_STANDARD 17)
target_link_libraries(rl-cpp "${TORCH_LIBRARIES}")

if (MSVC)
  file(GLOB TORCH_DLLS "${TORCH_INSTALL_PREFIX}/lib/*.dll")
  add_custom_command(TARGET rl-cpp
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_if_different
                     ${TORCH_DLLS}
                     $<TARGET_FILE_DIR:rl-cpp>)

  set(DEPS_DLLS "${CONDA_PREFIX}/bin/libiomp5md.dll"
                "${CONDA_PREFIX}/Library/bin/uv.dll")
  add_custom_command(TARGET rl-cpp
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${DEPS_DLLS}
                    $<TARGET_FILE_DIR:rl-cpp>)                  
endif (MSVC)