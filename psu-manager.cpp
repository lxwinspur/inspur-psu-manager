#include "config.h"

#include "psu-manager.hpp"

#include "psu-eeprom.hpp"
#include "type.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

namespace inpsur::psu::manager
{

namespace fs = std::filesystem;

std::string PsuManager::getHwmonFilename(const fs::path& path)
{
    std::regex expr{"hwmon\\d+$"};
    for (auto& file : fs::directory_iterator(path))
    {
        if (std::regex_search(file.path().string(), expr))
        {
            return file.path().filename();
        }
    }
    return std::string{};
}

PsuManager::PsuManager(const fs::path& path)
{
    for (auto& file : fs::directory_iterator(path))
    {
        std::string name = getValues(file.path() / "name");

        if (name == PSU_DRIVER_NAME)
        {
            fs::path hwmonPath = file.path() / "hwmon";
            if (!fs::exists(hwmonPath))
            {
                continue;
            }
            auto hwmonXX = getHwmonFilename(hwmonPath);
            if (hwmonXX.empty())
            {
                std::cerr << "Error hwmon file path";
                continue;
            }

            productInfo product;
            product.partNumber = getValues(hwmonPath / hwmonXX / "part_number");
            product.serialNumber =
                getValues(hwmonPath / hwmonXX / "serial_number");
            product.manufacturerName =
                getValues(hwmonPath / hwmonXX / "vendor");
            product.productName = getValues(hwmonPath / hwmonXX / "model");
            product.version = getValues(hwmonPath / hwmonXX / "hw_version");

            eeprom::PsuEEprom psu(&product);

            std::vector<uint8_t> data = psu.encodeEEprom();
            setEEprom(file.path() / "eeprom",
                      reinterpret_cast<const char*>(data.data()), data.size());
        }
    }
}

std::string PsuManager::getValues(const fs::path& path)
{
    std::ifstream ifs(path, std::ios::in);
    if (!ifs)
    {
        return "";
    }

    std::string value;
    ifs >> value;
    ifs.close();

    return value;
}

void PsuManager::setEEprom(fs::path path, const char* buf, size_t len)
{
    if (buf == nullptr)
    {
        return;
    }

    std::ofstream file(path, std::ios::out | std::ios::binary);
    if (!file)
    {
        std::cerr << "Erroe opening write file \n";
        return;
    }

    file.write(buf, len);
    file.flush();
    file.close();
}

} // namespace inpsur::psu::manager