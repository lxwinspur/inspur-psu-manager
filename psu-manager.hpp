#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <string>

namespace inpsur::psu::manager
{

namespace fs = std::filesystem;

using getValueFunction = std::function<std::string()>;

// std::map<std::string, std::string> fruNames = {
//     {"part_number", ""}, {"serial_number", ""}, {"vendor", ""},
//     {"model", ""},       {"mode", ""},          {"hw_version", ""},
//     {"fw_version", ""}};

class PsuManager
{
  public:
    PsuManager() = delete;
    ~PsuManager() = default;
    PsuManager(const PsuManager&) = delete;
    PsuManager& operator=(const PsuManager&) = delete;
    PsuManager(PsuManager&&) = delete;
    PsuManager& operator=(PsuManager&&) = delete;

    PsuManager(const fs::path& path);

  private:
    std::string getValues();
};

} // namespace inpsur::psu::manager