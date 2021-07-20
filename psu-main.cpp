
#include "config.h"

#include "psu-manager.hpp"

#include <iostream>

int main(void)
{
    std::cerr << "main start" << std::endl;
    inpsur::psu::manager::PsuManager psuManager(EEPROM_ROOT_PATH);

    return 0;
}