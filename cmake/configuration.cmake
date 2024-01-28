set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(VIRTUAL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/.mamba)

list(APPEND CMAKE_PREFIX_PATH ${VIRTUAL_PREFIX}/lib/site-packages)
include_directories(${VIRTUAL_PREFIX}/Library/include)
link_directories(${VIRTUAL_PREFIX}/Library/lib)
set(ENV{MKLROOT} ${VIRTUAL_PREFIX}/Library/lib)

macro(target_copy_if_different)
    cmake_parse_arguments(_COPY_IF_DIFFERENT
                          ""
                          "TARGET;DESTINATION"
                          "FILES"
                          ${ARGN})

    if($<BOOL:${_COPY_IF_DIFFERENT_FILES}>)
        add_custom_command(TARGET ${_COPY_IF_DIFFERENT_TARGET} POST_BUILD
                           COMMAND ${CMAKE_COMMAND} -E copy_if_different
                           ${_COPY_IF_DIFFERENT_FILES}
                           ${_COPY_IF_DIFFERENT_DESTINATION})
    endif()
endmacro(target_copy_if_different)
