#pragma once

#include <vector>
#include <string>

namespace SimpleBLE {
namespace Dongl {
namespace USB {

class UsbHelper {
  public:
    UsbHelper() = default;
    ~UsbHelper() = default;

    static std::vector<std::string> get_dongl_devices();

  private:
    static const uint16_t DONGL_VENDOR_ID = 0x0403;
    static const uint16_t DONGL_PRODUCT_ID = 0x6001;
};

}  // namespace USB
}  // namespace Dongl
}  // namespace SimpleBLE
