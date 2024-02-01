set(VIRTUAL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/.mamba)

list(APPEND CMAKE_PREFIX_PATH ${VIRTUAL_PREFIX}/Library)

include_directories(${VIRTUAL_PREFIX}/Library/include)
link_directories(${VIRTUAL_PREFIX}/Library/lib)
