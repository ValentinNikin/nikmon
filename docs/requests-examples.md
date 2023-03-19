Получить список агентов
method: GET
query: /api/agents
body: -

Добавить агент
method: POST
query: /api/agents
body:
{
    ip: string,
    machineName: string,
    heartbeat: int
}
examples:
{
    ip: "192.168.126.48",
    machineName: "debian-1",
    heartbeat: 3000
}
{
    ip: "192.168.115.14",
    machineName: "debian-2",
    heartbeat: 1000
}

Получить список задач агента
method: GET
query: /api/agents/{id}/tasks
body: -

Назначить агенту задачу
method: POST
query: /api/agents/{id}/tasks
body:
{
    frequency: int, (0 - Once, 1 - MultipleTimes)
    delay: int,
    key: string,
    valueType: int (0 - uint, 1 - float, 2 - text)
}
examples:
{
    frequency: 1,
    delay: 5000,
    key: "mem.usage.rss",
    valueType: 0
}
{
    frequency: 1,
    delay: 3000,
    key: "cpu.usage",
    valueType: 0
}

Отредактировать конфигурацию задачи для агента
method: PUT
query: /api/agents/{id}/tasks
body:
{
    id: string,
    frequency: int, (0 - Once, 1 - MultipleTimes)
    delay: int,
    key: string,
    valueType: int (0 - uint, 1 - float, 2 - text)
}
examples:
{
    id: "",
    frequency: 1,
    delay: 5000,
    key: "mem.usage.rss",
    valueType: 0
}
{
    id: "",
    frequency: 1,
    delay: 3000,
    key: "cpu.usage",
    valueType: 0
}

Приостановить выполнение задачи
method: PUT
query: /api/agents/{agentId}/tasks/{taskId}/toggle
body: -

Удалить задачу
method: DELETE
query: /api/tasks/{id}/result
body: -

Получить результаты задачи на определенном промежутке времени
method: GET
query: /api/tasks/{id}/result?from={from},to={to}
body: -

Получить информацию об ошибках в получении данных на определенном промежутке времени
method: GET
query: /api/tasks/{id}/error?from={from},to={to}
body: -