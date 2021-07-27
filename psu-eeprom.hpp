#pragma once

#include "type.hpp"

#include <string>
#include <vector>

namespace inpsur::psu::eeprom
{

class PsuEEprom
{
  public:
    PsuEEprom() = delete;
    ~PsuEEprom() = default;
    PsuEEprom(const PsuEEprom&) = delete;
    PsuEEprom& operator=(const PsuEEprom&) = delete;
    PsuEEprom(PsuEEprom&&) = delete;
    PsuEEprom& operator=(PsuEEprom&&) = delete;

    PsuEEprom(productInfo* product) : product(product)
    {}

    std::vector<uint8_t> encodeEEprom();

  private:
    fruCommonHdr getHeader();
    std::vector<uint8_t> getChassisInfo();
    std::vector<uint8_t> getBoardInfo();
    std::vector<uint8_t> getProductInfo();

    uint8_t crc(const uint8_t* data, size_t len);

  private:
    productInfo* product;
};

} // namespace inpsur::psu::eeprom
