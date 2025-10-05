#pragma once

#include <cstddef>
#include <cstdint>

namespace SimpleBLE {
namespace Dongl {
namespace CMD {

uint16_t calculate_crc(const uint8_t* src, size_t len);

}  // namespace CMD
}  // namespace Dongl
}  // namespace SimpleBLE