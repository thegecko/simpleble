#pragma once

#include "UsbHelperImpl.h"

namespace SimpleBLE {
namespace Dongl {
namespace USB {

class UsbHelperApple : public UsbHelperImpl {
  public:
    UsbHelperApple(const std::string& device_path);
    ~UsbHelperApple();

    void tx(const kvn::bytearray& data);
    void set_rx_callback(std::function<void(const kvn::bytearray&)> callback);

    static std::vector<std::string> get_dongl_devices();
};

}  // namespace USB
}  // namespace Dongl
}  // namespace SimpleBLE