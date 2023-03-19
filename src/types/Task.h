#pragma once

#include <string>

#include "types/enums/TaskFrequency.h"
#include "types/enums/TaskValueType.h"
#include "types/enums/TaskState.h"

#include "core/database/types/TaskDB.h"
#include "types/rest-api/EditTask.h"
#include "types/agent-communication/Command.h"
#include "types/agent-communication/CommandConfirmation.h"

namespace nikmon {
namespace types {

class Task {
public:

    Task(const EditTask& task);
    Task(const TaskDB& task);

    std::string getId() const;
    TaskStatus getStatus() const;

    std::unique_ptr<Command> constructCommand() const;

    void acceptChanges(const EditTask&);
    void acceptCancel();
    void acceptConfirmation(const CommandConfirmation&);

private:
    std::string _id;
    TaskFrequency _frequency;
    int _delay;
    std::string _key;
    TaskValueType _valueType;

    TaskState _state;
};

}
}