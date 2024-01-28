cmake_minimum_required(VERSION 3.27)

project(rl-cpp LANGUAGES CXX CUDA)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

include(cmake/configuration.cmake)
include(CMakeDependentOption)

include(cmake/compilation.cmake)
include(cmake/static-analysis.cmake)

find_package(Torch REQUIRED)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS})

add_executable(rl-cpp main.cpp)
target_link_libraries(rl-cpp PUBLIC compilation_options ${TORCH_LIBRARIES})
target_copy_if_different(TARGET rl-cpp
                         FILES $<TARGET_RUNTIME_DLLS:rl-cpp>
                         DESTINATION $<TARGET_FILE_DIR:rl-cpp>)

add_executable(echo-server echo_server.cpp)
target_link_libraries(echo-server PUBLIC compilation_options)
target_copy_if_different(TARGET echo-server
                         FILES $<TARGET_RUNTIME_DLLS:echo-server>
                         DESTINATION $<TARGET_FILE_DIR:echo-server>)

add_executable(echo-client echo_client.cpp)
target_link_libraries(echo-client PUBLIC compilation_options)
target_copy_if_different(TARGET echo-client
                         FILES $<TARGET_RUNTIME_DLLS:echo-client>
                         DESTINATION $<TARGET_FILE_DIR:echo-client>)

add_custom_target(echo)
add_dependencies(echo echo-server echo-client)
