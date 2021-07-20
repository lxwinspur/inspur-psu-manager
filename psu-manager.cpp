#include "psu-manager.hpp"

#include <iostream>

namespace inpsur::psu::manager
{
PsuManager::PsuManager(const fs::path& path)
{
    for (auto& file : fs::directory_iterator(path))
    {
        std::cerr << "file name = " << file << std::endl;
    }
}

} // namespace inpsur::psu::manager