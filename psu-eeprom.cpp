#include "psu-eeprom.hpp"

#include "type.hpp"

#include <string.h>

#include <iostream>

namespace inpsur::psu::eeprom
{

uint8_t PsuEEprom::crc(const uint8_t* data, size_t len)
{
    uint8_t sum = 0;
    size_t i;
    for (i = 0; i < len; i++)
        sum += data[i];

    return (-sum);
}

fruCommonHdr PsuEEprom::getHeader()
{
    const int FRU_FORMAT_VERSION = 0x01;

    struct fruCommonHdr hdr;
    hdr.fmtver = FRU_FORMAT_VERSION;
    hdr.internal = 0;
    hdr.chassis = 0;
    hdr.board = 0;
    hdr.product = sizeof(hdr) >> 3;
    hdr.multirec = 0;
    hdr.pad = 0;
    hdr.crc = crc((uint8_t*)&hdr, sizeof(hdr) - 1);

    return hdr;
}

std::vector<uint8_t> PsuEEprom::getChassisInfo()
{
    // TODO
    return {};
}

std::vector<uint8_t> PsuEEprom::getBoardInfo()
{
    // TODO
    return {};
}

static uint8_t customCopy(std::vector<uint8_t>& buf, uint8_t offset,
                          std::string value)
{
    uint8_t len = value.length();
    uint8_t index = offset;
    buf.resize(index + len + 1);

    uint8_t typeLen = len | 0xc0;
    memcpy(buf.data() + index, &typeLen, 1);
    index += 1;
    memcpy(buf.data() + index, value.c_str(), len);
    index += len;

    return index;
}

std::vector<uint8_t> PsuEEprom::getProductInfo()
{
    // Refer to:FRU spec Table 12-1
    std::vector<uint8_t> data(3, 0);

    uint8_t areaFormatVersion = 1;
    memcpy(data.data(), &areaFormatVersion, sizeof(areaFormatVersion));

    uint8_t areaLenth = 0;
    memcpy(data.data() + 1, &areaLenth, sizeof(areaLenth));

    uint8_t languageCode = 0;
    memcpy(data.data() + 2, &languageCode, sizeof(languageCode));

    uint8_t index = 3;

    index = customCopy(data, index, product->manufacturerName);
    index = customCopy(data, index, product->productName);
    index = customCopy(data, index, product->partNumber);
    index = customCopy(data, index, product->version);
    index = customCopy(data, index, product->serialNumber);
    index = customCopy(data, index, product->assertTag);
    index = customCopy(data, index, product->fruFileId);

    data.resize(index + 3);
    uint8_t fields = 0xc1;
    memcpy(data.data() + index, &fields, sizeof(fields));
    index += 1;

    uint8_t unsed = 0x0;
    memcpy(data.data() + index, &unsed, sizeof(unsed));
    index += 1;

    int sum = index + 1;
    data[1] = (sum + (8 - sum % 8)) / 8;
    index = sum + (8 - sum % 8) - 1;

    data.resize(index + 1);
    data[index] = crc(data.data(), data.size() - 1);

    return data;
}

std::vector<uint8_t> PsuEEprom::encodeEEprom()
{
    std::vector<uint8_t> data;
    fruCommonHdr hdr = getHeader();
    std::vector<uint8_t> payload = getProductInfo();

    data.resize(8 + payload.size());
    memcpy(data.data(), &hdr, sizeof(fruCommonHdr));
    memcpy(data.data() + sizeof(fruCommonHdr), payload.data(), payload.size());

    return data;
}

} // namespace inpsur::psu::eeprom