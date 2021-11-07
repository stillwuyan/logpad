#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <memory>
#include <fstream>
#include "nlohmann/json.hpp"

namespace window {
using json = nlohmann::json;

class Config
{
public:
    ~Config() = default;
    static Config& GetInstance()
    {
        return *StaticInstance();
    }

    std::string filter_tools;

private:
    Config() = delete;
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(Config&&) = delete;

    Config(const std::string& file)
    {
        std::ifstream f(file);
        try
        {
            _conf = json::parse(f);
            filter_tools = _conf["filter_tools"];
        }
        catch (const json::parse_error&)
        {
            filter_tools = "grep -n";
        }
    }

    static std::unique_ptr<Config>& StaticInstance()
    {
        static std::unique_ptr<Config> instance(new Config("config.yml"));
        return instance;
    }

    json _conf;
};

}   // end namespace
#endif  // CONFIG_HPP