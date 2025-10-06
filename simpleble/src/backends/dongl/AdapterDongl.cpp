#include <simpleble/Peripheral.h>

#include "AdapterDongl.h"
#include "BuilderBase.h"
#include "CommonUtils.h"
#include "PeripheralBase.h"
#include "PeripheralDongl.h"

#include "cmd/Commands.h"
#include "cmd/Events.h"
#include <memory>
#include <thread>

using namespace SimpleBLE;

bool AdapterDongl::bluetooth_enabled() { return true; }

AdapterDongl::AdapterDongl(const std::string& device_path) : _usb_helper(std::make_unique<Dongl::USB::UsbHelper>(device_path)) {
    fmt::print("Dongl adapter created with device path: {}\n", device_path);

    _usb_helper->set_rx_callback([this](const kvn::bytearray& data) {
        fmt::print("Received data: {}\n", data.toHex(true));

        auto event = Dongl::CMD::UartEvent::from_bytes(data);
        fmt::print("Received event: {}\n", event->to_string());
    });

    auto command = Dongl::CMD::UartReadVersionCommand();
    _usb_helper->tx(command.to_bytes());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

}

AdapterDongl::~AdapterDongl() {}

void* AdapterDongl::underlying() const { return nullptr; }

std::string AdapterDongl::identifier() { return "Dongl Adapter"; }

BluetoothAddress AdapterDongl::address() { return "AA:BB:CC:DD:EE:FF"; }

void AdapterDongl::power_on() {}

void AdapterDongl::power_off() {}

bool AdapterDongl::is_powered() { return true; }

void AdapterDongl::scan_start() {}

void AdapterDongl::scan_stop() {}

void AdapterDongl::scan_for(int timeout_ms) {
    scan_start();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
    scan_stop();
}

bool AdapterDongl::scan_is_active() { return false; }

SharedPtrVector<PeripheralBase> AdapterDongl::scan_get_results() { return {}; }

SharedPtrVector<PeripheralBase> AdapterDongl::get_paired_peripherals() { return {}; }
