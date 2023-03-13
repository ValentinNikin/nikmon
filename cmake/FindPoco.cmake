#
# Poco_FOUND
# Poco_INCLUDE_DIRS
# Poco_LIBRARIES

set(Poco_FOUND FALSE)

find_path(Poco_INCLUDE_DIRS Poco)
if (Poco_INCLUDE_DIRS)
    message(STATUS "Found: ${Poco_INCLUDE_DIRS}")
endif()

find_library(Foundation_LIB NAMES PocoFoundation)
if (Foundation_LIB)
    message(STATUS "Found: ${Foundation_LIB}")
endif()

find_library(Util_LIB NAMES PocoUtil)
if (Util_LIB)
    message(STATUS "Found: ${Util_LIB}")
endif()

find_library(JSON_LIB NAMES PocoJSON)
if (JSON_LIB)
    message(STATUS "Found: ${JSON_LIB}")
endif()

find_library(Net_LIB NAMES PocoNet)
if (Net_LIB)
    message(STATUS "Found: ${Net_LIB}")
endif()

find_library(NET_SSL_LIB NAMES PocoNetSSL)
if (NET_SSL_LIB)
    message(STATUS "Found: ${NET_SSL_LIB}")
endif()

find_library(DATA_LIB NAMES PocoData)
if (DATA_LIB)
    message(STATUS "Found: ${DATA_LIB}")
endif()

find_library(DATA_SQLITE_LIB NAMES PocoDataSQLite)
if (DATA_SQLITE_LIB)
    message(STATUS "Found: ${DATA_SQLITE_LIB}")
endif()

if(Poco_INCLUDE_DIRS)
    set(Poco_FOUND TRUE)
    set(Poco_LIBRARIES ${Foundation_LIB} ${Util_LIB} ${JSON_LIB} ${Net_LIB} ${NET_SSL_LIB} ${DATA_LIB} ${DATA_SQLITE_LIB})
    message(STATUS "Found: ${Poco_LIBRARIES} (${Poco_INCLUDE_DIRS})")
else()
    message(FATAL_ERROR "Error! LibPoco_INCLUDE_DIR Not found!")
    set(LibPoco_FOUND FALSE)
endif()

mark_as_advanced(Poco_FOUND)