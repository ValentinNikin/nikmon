
set(LIBRARY_NAME nikmon_lib)
set(EXAMPLE_EXECUTABLE_NAME nikmon)

configure_file(version.h.in version.h)

# Configure library
add_library(
        ${LIBRARY_NAME} OBJECT

        app/Application.h
        app/Application.cpp
        app/Router.h
        app/Router.cpp
        app/ServiceLocator.h
        app/ServiceLocator.cpp

        controllers/Controller.h
        controllers/Controller.cpp
        controllers/AgentsController.h
        controllers/AgentsController.cpp
        controllers/AgentCommunicatorController.h
        controllers/AgentCommunicatorController.cpp
        controllers/ControllerAdapter.h
        controllers/ControllerAdapter.cpp

        core/IAgentsManager.h
        core/IAgentCommunicator.h
        core/IDatabaseManager.h
        core/WorkflowManager.h
        core/WorkflowManager.cpp
        core/SQLiteDatabaseManager.h
        core/SQLiteDatabaseManager.cpp

        types/Command.h
        types/Command.cpp
        types/CommandConfirmation.h
        types/RegistrationRequest.h
        types/RegistrationResponse.h
        types/StatusRequest.h
        types/StatusResponse.h
        types/TaskItem.h
        types/enums/AgentStatus.h
        types/enums/CommandType.h
        types/enums/TaskAction.h
        types/enums/TaskFrequency.h
        types/enums/TaskStatus.h
        types/enums/TaskValueType.h

        core/database/types/AgentDB.h
        core/database/types/TaskDB.h
        core/database/types/TaskItemDB.h
        core/database/types/TaskItemErrorDB.h

        core/database/repositories/IAgentsRepository.h
        core/database/repositories/SQLiteAgentsRepository.h
        core/database/repositories/SQLiteAgentsRepository.cpp

        core/database/repositories/ITasksRepository.h
        core/database/repositories/SQLiteTasksRepository.h
        core/database/repositories/SQLiteTasksRepository.cpp

        core/database/repositories/ITasksItemsRepository.h
        core/database/repositories/SQLiteTasksItemsRepository.h

        core/database/repositories/ITasksItemsErrorsRepository.h
        core/database/repositories/SQLiteTasksItemsErrorsRepository.h
        core/database/repositories/SQLiteTasksItemsErrorsRepository.cpp
        )

set_target_properties(
        ${LIBRARY_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(
        ${LIBRARY_NAME}
        PRIVATE
        ${PROJECT_DEPS}
)

target_include_directories(
        ${LIBRARY_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/
        SYSTEM
        ${PROJECT_DEPS_INCLUDES}
)

# Configure executable
add_executable(${EXAMPLE_EXECUTABLE_NAME} main.cpp)

set_target_properties(
        ${EXAMPLE_EXECUTABLE_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(
        ${EXAMPLE_EXECUTABLE_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/
        SYSTEM
        ${PROJECT_DEPS_INCLUDES}
)

target_link_libraries(
        ${EXAMPLE_EXECUTABLE_NAME}
        PRIVATE
        ${LIBRARY_NAME}
        ${PROJECT_DEPS}
)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(unit-tests)
endif()

include(GNUInstallDirs)

install(TARGETS ${EXAMPLE_EXECUTABLE_NAME} RUNTIME DESTINATION bin)