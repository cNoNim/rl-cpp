find_package(SFML 2 COMPONENTS window REQUIRED)

find_package(OpenGL REQUIRED)

find_package(Glad CONFIG REQUIRED)

glad_add_library(glad_gl_core_mx_33 REPRODUCIBLE MX API gl:core=3.3)

add_executable(sfml-app sfml_app.cpp)
target_link_libraries(sfml-app
                      PUBLIC compilation_options
                      PRIVATE sfml-graphics ${OPENGL_gl_LIBRARY} glad_gl_core_mx_33)
if (MSVC)
    target_copy_if_different(TARGET sfml-app
                            FILES
                            ${VIRTUAL_PREFIX}/Library/bin/sfml-system-2.dll
                            ${VIRTUAL_PREFIX}/Library/bin/sfml-window-2.dll
                            DESTINATION $<TARGET_FILE_DIR:sfml-app>)
endif(MSVC)
