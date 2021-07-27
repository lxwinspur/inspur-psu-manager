
#include "config.h"

#include "psu-manager.hpp"

#include <iostream>

int main(void)
{
    inpsur::psu::manager::PsuManager psuManager(EEPROM_ROOT_PATH);

    return 0;
}