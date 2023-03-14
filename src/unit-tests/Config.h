#pragma once

#include <string>

/**
 * Хранит путь до файлов-ресурсов, используемых в тестах
 */
class Config {
private:
    Config();
    static Config* _instance;
    ~Config();
public:
    Config(Config&) = delete;
    void operator=(const Config&) = delete;
    static Config* getInstance();

    void setResourcesPath(const std::string& path);
    const std::string& getResourcesPath();

    static constexpr auto pathToResourcesVariableName = "UNIT_TESTS_RESOURCES_PATH";

private:
    std::string _resourcesPath;
};
