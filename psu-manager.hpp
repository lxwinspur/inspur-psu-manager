#pragma once

#include <filesystem>
#include <functional>
#include <map>
#include <string>

namespace inpsur::psu::manager
{

namespace fs = std::filesystem;

using getValueFunction = std::function<std::string()>;

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
    std::string getValues(const fs::path& path);
    std::string getValues();
    void setEEprom(fs::path path, const char* buf, size_t len);
    std::string getHwmonFilename(const fs::path& path);
};

} // namespace inpsur::psu::manager