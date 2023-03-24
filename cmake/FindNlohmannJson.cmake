#
# NlohmannJson_FOUND
# NlohmannJson_INCLUDE_DIRS

set(NlohmannJson_FOUND FALSE)

find_path(NlohmannJson_INCLUDE_DIRS nlohmann-json/json.hpp
        NO_DEFAULT_PATH
        PATHS
        ${CMAKE_SOURCE_DIR}/3rdparty/)

if (NlohmannJson_INCLUDE_DIRS)
    message(STATUS "Found: ${NlohmannJson_INCLUDE_DIRS}")
    set(NlohmannJson_FOUND TRUE)
else()
    message(FATAL_ERROR "nlohmann-json library not found! ([${NlohmannJson_INCLUDE_DIRS}])")
endif()