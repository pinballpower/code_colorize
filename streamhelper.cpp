#include "streamhelper.h"

uint32_t read_u32_be(std::istream& f)
{
    uint32_t val;
    char bytes[4];
    f.read(bytes, 4);

    val = bytes[3] | (bytes[2] << 24) | (bytes[1] << 16) | (bytes[0] << 8);

    return val;
};

uint16_t read_u16_be(std::istream& f)
{
    uint16_t val;
    char bytes[2];
    f.read(bytes, 2);

    val = bytes[1] | (bytes[0] << 8);

    return val;
};


uint8_t read_u8(std::istream& f)
{
    uint8_t val;
    f.read((char*)&val, 1);
    return val;
};
