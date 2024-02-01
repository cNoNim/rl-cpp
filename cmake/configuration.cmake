set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

macro(target_copy_if_different)
    cmake_parse_arguments(_COPY_IF_DIFFERENT
                          ""
                          "TARGET;DESTINATION"
                          "FILES"
                          ${ARGN})

    if (DEFINED _COPY_IF_DIFFERENT_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unexpected parameters: ${_COPY_IF_DIFFERENT_UNPARSED_ARGUMENTS}")
    endif()

    set(have_files $<BOOL:${_COPY_IF_DIFFERENT_FILES}>>)
    if(have_files)
        add_custom_command(TARGET ${_COPY_IF_DIFFERENT_TARGET} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different
                           ${_COPY_IF_DIFFERENT_FILES}
                           ${_COPY_IF_DIFFERENT_DESTINATION}
                           COMMAND_EXPAND_LISTS)
    endif()
endmacro(target_copy_if_different)
