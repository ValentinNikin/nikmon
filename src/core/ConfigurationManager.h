#pragma once

#include <memory>

#include <Poco/Util/PropertyFileConfiguration.h>

class ConfigurationManager {
public:
    ConfigurationManager();

    void loadConfig(const std::string& path);

    bool hasKey(const std::string& key);
    int getInt(const std::string& key);
    std::string getString(const std::string& key);

private:
    Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> _config;
};