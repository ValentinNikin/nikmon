#
# Hypodermic_FOUND
# Hypodermic_INCLUDE_DIRS
# Hypodermic_LIBRARIES

set(Hypodermic_FOUND FALSE)

find_path(Hypodermic_INCLUDE_DIRS Hypodermic
        NO_DEFAULT_PATH
        PATHS
        ${CMAKE_SOURCE_DIR}/3rdparty/Hypodermic/)

if (Hypodermic_INCLUDE_DIRS)
    message(STATUS "Found: ${Hypodermic_INCLUDE_DIRS}")
    set(Hypodermic_FOUND TRUE)
else()
    message(FATAL_ERROR "Hypodermic library not found! ([${Hypodermic_INCLUDE_DIRS}])")
endif()