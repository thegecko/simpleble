#include "PeripheralDongl.h"

#include "CharacteristicBase.h"
#include "DescriptorBase.h"
#include "ServiceBase.h"

#include <simpleble/Exceptions.h>

#include <memory>
#include <thread>

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
    if (is_connected()) {
        return;
    }

    bool connection_successful = false;
    for (int i = 0; i < 5; i++) {
        connection_successful = _attempt_connect();

        if (connection_successful) {
            break;
        } else {
            std::this_thread::sleep_for(250ms);
        }
    }

    if (!connection_successful) {
        throw Exception::OperationFailed(fmt::format("Connection failed to be established"));
    }

    SAFE_CALLBACK_CALL(this->_callback_on_connected);
    fmt::print("PeripheralDongl::connect: connected\n");
}

void PeripheralDongl::disconnect() {
    if (!is_connected()) {
        return;
    }

    auto response = _serial_protocol->simpleble_disconnect(_conn_handle);
    if (response.ret_code != 0) {
        throw Exception::OperationFailed(fmt::format("Failed to disconnect: {}", response.ret_code));
    }

    // Wait for the disconnection to be confirmed.
    std::unique_lock<std::mutex> lock(disconnection_mutex_);
    disconnection_cv_.wait_for(lock, 250ms, [this]() { return !is_connected(); });

    if (is_connected()) {
        _conn_handle = BLE_CONN_HANDLE_INVALID;
        throw Exception::OperationFailed(fmt::format("Timeout while waiting for disconnection confirmation"));
    }
    SAFE_CALLBACK_CALL(this->_callback_on_disconnected);
}

bool PeripheralDongl::is_connected() { return _conn_handle != BLE_CONN_HANDLE_INVALID; }

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

uint16_t PeripheralDongl::conn_handle() const { return _conn_handle; }

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

bool PeripheralDongl::_attempt_connect() {
    auto response = _serial_protocol->simpleble_connect(static_cast<simpleble_BluetoothAddressType>(_address_type), _address);
    if (response.ret_code != 0) {
        SIMPLEBLE_LOG_ERROR(fmt::format("Error when attempting to connect: {}", response.ret_code));
        return false;
    }

    // NOTE: Bluetooth connections are non-acknowledged by the peripheral. The connected event that we get
    //       is just the confirmation that the connection packet was sent, not that it was received.
    //       Our only option is to wait a bit after the connection event is received to see if maybe a
    //       disconnection event arises too, in which case the connection failed.

    // Wait for the connection to be confirmed.
    {
        std::unique_lock<std::mutex> lock(connection_mutex_);
        connection_cv_.wait_for(lock, 1000ms, [this]() { return is_connected(); });
        if (!is_connected()) {
            SIMPLEBLE_LOG_ERROR("Timeout while waiting for connection confirmation");
            return false;
        }
    }

    // Wait a bit longer to confirm that no disconnection event arrives.
    {
        std::unique_lock<std::mutex> lock(disconnection_mutex_);
        disconnection_cv_.wait_for(lock, 500ms, [this]() { return !is_connected(); });
        if (!is_connected()) {
            SIMPLEBLE_LOG_ERROR("Connection failed to be established");
            return false;
        }
    }

    return true;
}

void PeripheralDongl::notify_connected(uint16_t conn_handle) {
    fmt::print("PeripheralDongl::notify_connected: {}\n", conn_handle);
    _conn_handle = conn_handle;
    connection_cv_.notify_all();
}

void PeripheralDongl::notify_disconnected() {
    fmt::print("PeripheralDongl::notify_disconnected: {}\n", _conn_handle);
    _conn_handle = BLE_CONN_HANDLE_INVALID;
    disconnection_cv_.notify_all();

    // TODO: Only throw the callback if the disconection was unexpected.
    //SAFE_CALLBACK_CALL(this->_callback_on_disconnected);
}