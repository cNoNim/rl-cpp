find_package(Torch REQUIRED)

add_executable(rl-cpp main.cpp)
target_link_libraries(rl-cpp PUBLIC compilation_options ${TORCH_LIBRARIES})
target_compile_options(rl-cpp
                       PRIVATE  $<$<COMPILE_LANGUAGE:CXX>:${TORCH_CXX_FLAGS}>)
target_copy_if_different(TARGET rl-cpp
                         FILES $<TARGET_RUNTIME_DLLS:rl-cpp>
                         DESTINATION $<TARGET_FILE_DIR:rl-cpp>)
