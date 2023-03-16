
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
        controllers/TasksController.h
        controllers/TasksController.cpp

        core/IAgentsManager.h
        core/IAgentCommunicator.h
        core/TasksManager.h
        core/WorkflowManager.h
        core/WorkflowManager.cpp
        core/TasksManager.h
        core/TasksManager.cpp

        core/database/IDatabaseManager.h
        core/database/SQLiteDatabaseManager.h
        core/database/SQLiteDatabaseManager.cpp

        types/agent-communication/Command.h
        types/agent-communication/Command.cpp
        types/agent-communication/CommandConfirmation.h
        types/agent-communication/RegistrationRequest.h
        types/agent-communication/RegistrationResponse.h
        types/agent-communication/StatusRequest.h
        types/agent-communication/StatusResponse.h
        types/agent-communication/StatusResponse.cpp
        types/agent-communication/TaskItem.h

        types/rest-api/AgentShortInfo.h
        types/rest-api/TaskShortInfo.h
        types/rest-api/EditTask.h
        types/rest-api/EditAgent.h
        types/rest-api/TaskShortInfo.h
        types/rest-api/TaskErrorItem.h
        types/rest-api/TaskResultItem.h
        types/rest-api/TaskInfo.h
        types/rest-api/AgentInfo.h


        types/enums/AgentStatus.h
        types/enums/CommandType.h
        types/enums/TaskAction.h
        types/enums/TaskFrequency.h
        types/enums/TaskResultStatus.h
        types/enums/TaskStatus.h
        types/enums/TaskValueType.h

        core/database/types/AgentDB.h
        core/database/types/TaskDB.h
        core/database/types/TaskItemDB.h
        core/database/types/TaskItemErrorDB.h

        core/database/repositories/IAgentsRepository.h
        core/database/repositories/sqlite/SQLiteAgentsRepository.h
        core/database/repositories/sqlite/SQLiteAgentsRepository.cpp

        core/database/repositories/ITasksRepository.h
        core/database/repositories/sqlite/SQLiteTasksRepository.h
        core/database/repositories/sqlite/SQLiteTasksRepository.cpp

        core/database/repositories/ITasksItemsRepository.h
        core/database/repositories/sqlite/SQLiteTasksItemsRepository.h

        core/database/repositories/ITasksItemsErrorsRepository.h
        core/database/repositories/sqlite/SQLiteTasksItemsErrorsRepository.h
        core/database/repositories/sqlite/SQLiteTasksItemsErrorsRepository.cpp

        utils/Utils.h
        utils/Utils.cpp
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