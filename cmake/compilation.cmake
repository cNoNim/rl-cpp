if(compilation_included)
    return()
endif()
set(compilation_included true)

if (MSVC)
  add_definitions(-D_WIN32_WINDOWS=0x0A00 -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

add_library(compilation_options INTERFACE)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(WARNING_AS_ERROR "Treats compiler warnings as errors" ON)
option(USE_LTO "Enable Link Time Optimization" OFF)
option(USE_PCH "Enable Precompiled Headers" OFF)
option(USE_ASAN "Enable the Address Sanitizers" OFF)
option(USE_TSAN "Enable the Thread Sanitizers" OFF)
option(USE_MSAN "Enable the Memory Sanitizers" OFF)
option(USE_UBSAN "Enable the Undefined Behavior Sanitizers" OFF)

find_program(CCACHE ccache)
if (CCACHE)
    message("Use ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

if(MSVC)
    target_compile_options(compilation_options INTERFACE /W4 "/permissive-")
    if(WARNING_AS_ERROR)
        target_compile_options(compilation_options INTERFACE /WX)
    endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(compilation_options
                           INTERFACE -Wall
                                     -Wextra
                                     -Wshadow
                                     -Wnon-virtual-dtor
                                     -Wold-style-cast
                                     -Wcast-align
                                     -Wunused
                                     -Woverloaded-virtual
                                     -Wpedantic
                                     -Wconversion
                                     -Wsign-conversion
                                     -Wnull-dereference
                                     -Wdouble-promotion
                                     -Wformat=2)
    if(WARNING_AS_ERROR)
        target_compile_options(compilation_options INTERFACE -Werror)
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(compilation_options
                               INTERFACE -Wmisleading-indentation
                                         -Wduplicated-cond
                                         -Wduplicated-branches
                                         -Wlogical-op
                                         -Wuseless-cast)
    endif()
endif()

if (USE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result OUTPUT output)
    if (result)
        message("Use Link Time Optimisation")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else ()
        message(SEND_ERROR "LTO is not supported: ${output}")
    endif ()
endif ()

if (USE_PCH)
    message("Use Address Sanitizer")
    target_precompile_headers(compilation_options
                              INTERFACE <algorithm>
                                        <array>
                                        <vector>
                                        <string>
                                        <utility>
                                        <functional>
                                        <memory>
                                        <memory_resource>
                                        <string_view>
                                        <cmath>
                                        <cstddef>
                                        <type_traits>)
endif ()

if(USE_ASAN)
    message("Use Address Sanitizer")
    target_compile_options(compilation_options INTERFACE -fsanitize=address -fno-omit-frame-pointer)
    target_link_libraries(compilation_options INTERFACE -fsanitize=address)
endif()

if(USE_TSAN)
    message("Use Thread Sanitizer")
    target_compile_options(compilation_options INTERFACE -fsanitize=thread)
    target_link_libraries(compilation_options INTERFACE -fsanitize=thread)
endif()

if(USE_MSAN)
    message("Use Memory Sanitizer")
    target_compile_options(compilation_options INTERFACE -fsanitize=memory -fno-omit-frame-pointer)
    target_link_libraries(compilation_options INTERFACE -fsanitize=memory)
endif()

if(USE_UBSAN)
    message("Use Undefined Behavior Sanitizer")
    target_compile_options(compilation_options INTERFACE -fsanitize=undefined)
    target_link_libraries(compilation_options INTERFACE -fsanitize=undefined)
endif()
