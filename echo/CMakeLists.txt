add_executable(echo-server echo_server.cpp)
target_link_libraries(echo-server
                      PUBLIC compilation_options
                      PRIVATE extensions)
target_copy_if_different(TARGET echo-server
                         FILES $<TARGET_RUNTIME_DLLS:echo-server>
                         DESTINATION $<TARGET_FILE_DIR:echo-server>)

add_executable(echo-client echo_client.cpp)
target_link_libraries(echo-client
                      PUBLIC compilation_options
                      PRIVATE extensions)
target_copy_if_different(TARGET echo-client
                         FILES $<TARGET_RUNTIME_DLLS:echo-client>
                         DESTINATION $<TARGET_FILE_DIR:echo-client>)

add_custom_target(echo)
add_dependencies(echo echo-server echo-client)
