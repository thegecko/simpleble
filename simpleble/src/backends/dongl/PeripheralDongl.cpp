#include "PeripheralDongl.h"

#include "CharacteristicBase.h"
#include "DescriptorBase.h"
#include "ServiceBase.h"

#include <simpleble/Exceptions.h>

#include <memory>

#include "CommonUtils.h"
#include "LoggingInternal.h"

using namespace SimpleBLE;
using namespace std::chrono_literals;

PeripheralDongl::PeripheralDongl() {}

PeripheralDongl::~PeripheralDongl() {}

void* PeripheralDongl::underlying() const { return nullptr; }

std::string PeripheralDongl::identifier() { return "Dongl Peripheral"; }

BluetoothAddress PeripheralDongl::address() { return "11:22:33:44:55:66"; }

BluetoothAddressType PeripheralDongl::address_type() { return BluetoothAddressType::PUBLIC; };

int16_t PeripheralDongl::rssi() { return -60; }

int16_t PeripheralDongl::tx_power() { return 5; }

uint16_t PeripheralDongl::mtu() { return 0; }

void PeripheralDongl::connect() {}

void PeripheralDongl::disconnect() {}
bool PeripheralDongl::is_connected() { return false; }

bool PeripheralDongl::is_connectable() { return true; }

bool PeripheralDongl::is_paired() { return false; }

void PeripheralDongl::unpair() {}

SharedPtrVector<ServiceBase> PeripheralDongl::available_services() { return {}; }

SharedPtrVector<ServiceBase> PeripheralDongl::advertised_services() { return {}; }

std::map<uint16_t, ByteArray> PeripheralDongl::manufacturer_data() { return {}; }

ByteArray PeripheralDongl::read(BluetoothUUID const& service, BluetoothUUID const& characteristic) { return {}; }

void PeripheralDongl::write_request(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                                    ByteArray const& data) {}

void PeripheralDongl::write_command(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                                    ByteArray const& data) {}

void PeripheralDongl::notify(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                             std::function<void(ByteArray payload)> callback) {}

void PeripheralDongl::indicate(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                               std::function<void(ByteArray payload)> callback) {}

void PeripheralDongl::unsubscribe(BluetoothUUID const& service, BluetoothUUID const& characteristic) {}

ByteArray PeripheralDongl::read(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                                BluetoothUUID const& descriptor) {
    return {};
}

void PeripheralDongl::write(BluetoothUUID const& service, BluetoothUUID const& characteristic,
                            BluetoothUUID const& descriptor, ByteArray const& data) {}

void PeripheralDongl::set_callback_on_connected(std::function<void()> on_connected) {
    if (on_connected) {
        callback_on_connected_.load(std::move(on_connected));
    } else {
        callback_on_connected_.unload();
    }
}

void PeripheralDongl::set_callback_on_disconnected(std::function<void()> on_disconnected) {
    if (on_disconnected) {
        callback_on_disconnected_.load(std::move(on_disconnected));
    } else {
        callback_on_disconnected_.unload();
    }
}
