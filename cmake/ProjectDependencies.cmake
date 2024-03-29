set(PROJECT_DEPS)
set(PROJECT_DEPS_INCLUDES)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Poco REQUIRED Foundation Util JSON Net NetSSL Data)
list(APPEND PROJECT_DEPS_INCLUDES ${Poco_INCLUDE_DIRS})
list(APPEND PROJECT_DEPS ${Poco_LIBRARIES})

find_package(Hypodermic REQUIRED)
list(APPEND PROJECT_DEPS_INCLUDES ${Hypodermic_INCLUDE_DIRS})

find_package(NlohmannJson REQUIRED)
list(APPEND PROJECT_DEPS_INCLUDES ${NlohmannJson_INCLUDE_DIRS})

find_package(Boost COMPONENTS program_options filesystem regex REQUIRED)

list(APPEND PROJECT_DEPS_INCLUDES "${CMAKE_SOURCE_DIR}/3rdparty/")

list(APPEND PROJECT_DEPS ${CMAKE_THREAD_LIBS_INIT})