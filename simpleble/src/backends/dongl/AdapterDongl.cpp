#include <simpleble/Peripheral.h>

#include "AdapterDongl.h"
#include "BuilderBase.h"
#include "CommonUtils.h"
#include "serial/Protocol.h"
#include "PeripheralBase.h"
#include "PeripheralDongl.h"

// #include "cmd/Commands.h"
// #include "cmd/Events.h"
#include <memory>
#include <thread>

using namespace SimpleBLE;

bool AdapterDongl::bluetooth_enabled() { return true; }

AdapterDongl::AdapterDongl(const std::string& device_path)
    : _serial_protocol(std::make_unique<Dongl::Serial::Protocol>(device_path)) {
    fmt::print("Dongl adapter created with device path: {}\n", device_path);

    // _serial_protocol->set_response_callback([this](const dongl_Response& response) {
    //     fmt::print("Received response: {} bytes\n", response.size());

    //     // TODO: Process the received response
    //     // auto event = Dongl::CMD::UartEvent::from_bytes(response);
    //     // fmt::print("Received event: {}\n", event->to_string());
    // });

    // _serial_protocol->set_event_callback([this](const std::vector<uint8_t>& event) {
    //     fmt::print("Received event: {} bytes\n", event.size());

    //     // TODO: Process the received event
    // });

    // _serial_protocol->set_error_callback([this](const std::vector<uint8_t>& error) {
    //     fmt::print("Protocol error: {} bytes\n", error.size());

    //     // TODO: Handle protocol errors
    // });

    // TODO: Send initialization commands
    // auto command = Dongl::CMD::UartReadVersionCommand();
    // _serial_protocol->send_packet(command.to_bytes());

    auto response = _serial_protocol->basic_whoami();
    fmt::print("Whoami: {}\n", response.whoami);

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
