#include "ConfigurationManager.h"

ConfigurationManager::ConfigurationManager()
    : _config(new Poco::Util::PropertyFileConfiguration()) {}

void ConfigurationManager::loadConfig(const std::string& path) {
    _config->load(path);
}

bool ConfigurationManager::hasKey(const std::string& key) {
    return _config->has(key);
}

int ConfigurationManager::getInt(const std::string& key) {
    return _config->getInt(key);
}

std::string ConfigurationManager::getString(const std::string& key) {
    return _config->getString(key);
}