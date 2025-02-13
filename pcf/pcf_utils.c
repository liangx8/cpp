#include <stdint.h>

uint32_t msbyte4(uint8_t b[])
{
    return (b[0] << 24) + (b[1] << 16) + (b[2]<< 8) + b[3];
}