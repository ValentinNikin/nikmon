#include "Task.h"

#include <Poco/UUIDGenerator.h>

using namespace nikmon::types;

Task::Task(const EditTask& task)
        : _id(Poco::UUIDGenerator().createRandom().toString()),
        _frequency(task.frequency),
        _delay(task.delay),
        _key(task.key),
        _valueType(task.valueType),
        _state(TaskState::Creating) {}

Task::Task(const TaskDB& task)
        : _id(task.id),
        _frequency(task.frequency),
        _delay(task.delay),
        _key(task.key),
        _valueType(task.valueType),
        _state(TaskState::Creating) {}

std::string Task::getId() const {
    return _id;
}

TaskStatus Task::getStatus() const {
    return _state == TaskState::Running ? TaskStatus::Active : TaskStatus::Pending;
}

std::unique_ptr<Command> Task::constructCommand() const {
    if (_state == TaskState::Running) {
        return nullptr;
    }

    auto command = std::make_unique<Command>();
    command->taskId = _id;
    if (_state == TaskState::Creating) {
        command->type = CommandType::Add;
        command->payload.key = _key;
        command->payload.frequency = _frequency;
        command->payload.delay = _delay;
    }
    else if (_state == TaskState::Changing) {
        command->type = CommandType::Change;
        command->payload.key = _key;
        command->payload.frequency = _frequency;
        command->payload.delay = _delay;
    }
    else if (_state == TaskState::Cancelling) {
        command->type = CommandType::Cancel;
    }

    return command;
}

void Task::acceptChanges(const EditTask& task) {
    if (_state != TaskState::Running) {
        throw std::runtime_error("Task is busy, please wait while the previous operation is completed");
    }

    _state = TaskState::Changing;

    _frequency = task.frequency;
    _delay = task.delay;
    _key = task.key;
    _valueType = task.valueType;
}

void Task::acceptCancel() {
    if (_state != TaskState::Running) {
        throw std::runtime_error("Task is busy, please wait while the previous operation is completed");
    }

    _state = TaskState::Cancelling;
}

void Task::acceptConfirmation(const CommandConfirmation&) {
    _state = TaskState::Running;
}