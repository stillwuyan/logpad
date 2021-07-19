#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <memory>
#include "yaml-cpp/yaml.h"

namespace window {

class Config
{
public:
    ~Config() = default;
    static Config& GetInstance()
    {
        return *staticInstance();
    }

    std::string filter_tools

private:
    Config() = delete;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(Config&&) = delete;

    Config(const std::string& file)
    {
        _conf = YAML::LoadFile(file);
        filter_tools = config["filter_tools"].as<std::string>();
    }

    static std::unique_ptr<Config>& StaticInstance()
    {
        static auto instance = std::make_unique<Config>("config.yml");
        return instance;
    }

    YAML::Node _conf;
};

}   // end namespace
#endif  // CONFIG_HPP