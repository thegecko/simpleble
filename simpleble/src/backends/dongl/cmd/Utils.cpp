#include "Utils.h"

namespace SimpleBLE {
namespace Dongl {
namespace CMD {

uint16_t calculate_crc(const uint8_t* src, size_t len) {
    const uint16_t poly = 0xA001;
    uint16_t crc = 0xFFFF;
    size_t i, j;

    for (i = 0; i < len; i++) {
        crc ^= (uint16_t)src[i];

        for (j = 0; j < 8; j++) {
            if (crc & 0x0001UL) {
                crc = (crc >> 1U) ^ poly;
            } else {
                crc = crc >> 1U;
            }
        }
    }

    return crc;
}

}  // namespace CMD
}  // namespace Dongl
}  // namespace SimpleBLE