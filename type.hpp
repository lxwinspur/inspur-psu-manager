#pragma once

#include <string>
#include <vector>

struct fruCommonHdr
{
    uint8_t fmtver;
    uint8_t internal;
    uint8_t chassis;
    uint8_t board;
    uint8_t product;
    uint8_t multirec;
    uint8_t pad;
    uint8_t crc;
} __attribute__((packed));

struct productInfo
{
    std::string manufacturerName;
    std::string productName;
    std::string partNumber;
    std::string version;
    std::string serialNumber;
    std::string assertTag;
    std::string fruFileId;
};
