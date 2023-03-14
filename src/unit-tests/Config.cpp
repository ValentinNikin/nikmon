#include "Config.h"

Config* Config::_instance = nullptr;

Config* Config::getInstance() {
    if (_instance == nullptr) {
        _instance = new Config();
    }
    return _instance;
}

Config::Config() {
    if (auto env = std::getenv(pathToResourcesVariableName)) {
        _resourcesPath = env;
    }
}

Config::~Config() {
    delete _instance;
}

void Config::setResourcesPath(const std::string& path) {
    _resourcesPath = path;
}

const std::string& Config::getResourcesPath() {
    return _resourcesPath;
}