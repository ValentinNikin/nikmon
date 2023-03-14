
set(UNIT_TEST_EXECUTABLE_NAME unit_tests)

find_package(GTest REQUIRED)

add_executable(
        ${UNIT_TEST_EXECUTABLE_NAME}
        main.cpp

        Config.h
        Config.cpp

        Utils.h
        Utils.cpp

        tests/UnittestSQLiteAgentsRepository.cpp
        tests/UnittestSQLiteTasksRepository.cpp
        tests/UnittestSQLiteTasksItemsRepository.cpp
        tests/UnittestSQLiteTasksItemsErrorsRepository.cpp
)

target_include_directories(
    ${UNIT_TEST_EXECUTABLE_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../
    ${CMAKE_CURRENT_SOURCE_DIR}/
    SYSTEM
    ${PROJECT_DEPS_INCLUDES}
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(
    ${UNIT_TEST_EXECUTABLE_NAME} 
    PRIVATE
    ${LIBRARY_NAME}
    ${PROJECT_DEPS}
    ${GTEST_BOTH_LIBRARIES}
)

add_test(
    NAME test-all
    COMMAND ${UNIT_TEST_EXECUTABLE_NAME}
)