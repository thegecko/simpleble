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

PeripheralDongl::PeripheralDongl(std::shared_ptr<Dongl::Serial::Protocol> serial_protocol, advertising_data_t advertising_data) {
    _serial_protocol = serial_protocol;
    _address_type = advertising_data.address_type;
    _identifier = advertising_data.identifier;
    _address = advertising_data.mac_address;
    _connectable = advertising_data.connectable;
    update_advertising_data(advertising_data);
}

PeripheralDongl::~PeripheralDongl() {}

void* PeripheralDongl::underlying() const { return nullptr; }

std::string PeripheralDongl::identifier() { return _identifier; }

BluetoothAddress PeripheralDongl::address() { return _address; }

BluetoothAddressType PeripheralDongl::address_type() { return _address_type; }

int16_t PeripheralDongl::rssi() { return _rssi; }

int16_t PeripheralDongl::tx_power() { return _tx_power; }

uint16_t PeripheralDongl::mtu() { return 0; }

void PeripheralDongl::connect() {
    auto response = _serial_protocol->simpleble_connect(static_cast<simpleble_BluetoothAddressType>(_address_type), _address);
    fmt::print("Connect: {}\n", response.ret_code);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}

void PeripheralDongl::disconnect() {
    auto response = _serial_protocol->simpleble_disconnect(0);
    fmt::print("Disconnect: {}\n", response.ret_code);
}

bool PeripheralDongl::is_connected() { return false; }

bool PeripheralDongl::is_connectable() { return _connectable; }

bool PeripheralDongl::is_paired() { return false; }

void PeripheralDongl::unpair() {}

SharedPtrVector<ServiceBase> PeripheralDongl::available_services() { return {}; }

SharedPtrVector<ServiceBase> PeripheralDongl::advertised_services() {
    SharedPtrVector<ServiceBase> service_list;
    for (auto& [service_uuid, data] : _service_data) {
        service_list.push_back(std::make_shared<ServiceBase>(service_uuid, data));
    }

    return service_list;
}

std::map<uint16_t, ByteArray> PeripheralDongl::manufacturer_data() { return _manufacturer_data; }

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
        _callback_on_connected.load(std::move(on_connected));
    } else {
        _callback_on_connected.unload();
    }
}

void PeripheralDongl::set_callback_on_disconnected(std::function<void()> on_disconnected) {
    if (on_disconnected) {
        _callback_on_disconnected.load(std::move(on_disconnected));
    } else {
        _callback_on_disconnected.unload();
    }
}

void PeripheralDongl::update_advertising_data(advertising_data_t advertising_data) {
    if (advertising_data.identifier != "") {
        _identifier = advertising_data.identifier;
    }
    _rssi = advertising_data.rssi;
    _tx_power = advertising_data.tx_power;
    _manufacturer_data = advertising_data.manufacturer_data;

    advertising_data.service_data.merge(_service_data);
    _service_data = advertising_data.service_data;
}