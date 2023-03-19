#include "ServiceLocator.h"

#include "controllers/AgentsController.h"
#include "controllers/AgentCommunicatorController.h"
#include "controllers/TasksController.h"
#include "core/IAgentsManager.h"
#include "core/IAgentCommunicator.h"
#include "core/WorkflowManager.h"
#include "core/database/IDatabaseManager.h"
#include "core/database/SQLiteDatabaseManager.h"
#include "core/ITasksManager.h"
#include "core/TasksManager.h"

std::unique_ptr<ServiceLocator> ServiceLocator::_instance = nullptr;

ServiceLocator* ServiceLocator::getInstance() {
    if (_instance == nullptr) {
        _instance = std::unique_ptr<ServiceLocator>(new ServiceLocator());
        _instance->init();
    }

    return _instance.get();
}

void ServiceLocator::init() {
    Hypodermic::ContainerBuilder builder;

    builder.registerType<AgentsController>();
    builder.registerType<AgentCommunicatorController>();
    builder.registerType<TasksController>();
    builder.registerType<WorkflowManager>()
            .as<IAgentCommunicator>()
            .as<IAgentsManager>()
            .singleInstance();
    builder.registerType<TasksManager>()
            .as<ITasksManager>()
            .singleInstance();
    builder.registerType<SQLiteDatabaseManager>()
            .as<IDatabaseManager>()
            .singleInstance();

    _container = builder.build();
}