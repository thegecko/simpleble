#include "PeripheralDongl.h"

#include "CharacteristicBase.h"
#include "DescriptorBase.h"
#include "ServiceBase.h"

#include <simpleble/Exceptions.h>

#include <memory>
#include <thread>

#include "CommonUtils.h"
#include "LoggingInternal.h"
#include "simpleble/Types.h"

#include <fmt/core.h>

using namespace SimpleBLE;
using namespace std::chrono_literals;

PeripheralDongl::PeripheralDongl(std::shared_ptr<Dongl::Serial::Protocol> serial_protocol,
                                 advertising_data_t advertising_data) {
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
    for (int i = 0; i < 10; i++) {
        fmt::print("PeripheralDongl::connect: attempt {}\n", i);
        connection_successful = _attempt_connect();
        fmt::print("PeripheralDongl::connect: attempt {} - success: {}\n", i, connection_successful);
        if (connection_successful) {
            break;
        } else {
            std::this_thread::sleep_for(750ms);
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
    disconnection_cv_.wait_for(lock, 500ms, [this]() { return !is_connected(); });

    if (is_connected()) {
        _conn_handle = BLE_CONN_HANDLE_INVALID;
        throw Exception::OperationFailed(fmt::format("Timeout while waiting for disconnection confirmation"));
    }
    SAFE_CALLBACK_CALL(this->_callback_on_disconnected);
}

bool PeripheralDongl::is_connected() {
    return _conn_handle != BLE_CONN_HANDLE_INVALID;
}

bool PeripheralDongl::is_connectable() { return _connectable; }

bool PeripheralDongl::is_paired() { return false; }

void PeripheralDongl::unpair() {}

SharedPtrVector<ServiceBase> PeripheralDongl::available_services() {
    SharedPtrVector<ServiceBase> service_list;
    for (auto& [service_handle, service] : _services) {
        SharedPtrVector<CharacteristicBase> characteristic_list;
        for (auto& [char_handle, characteristic] : service.characteristics) {
            SharedPtrVector<DescriptorBase> descriptor_list;
            for (auto& [desc_handle, descriptor] : characteristic.descriptors) {
                descriptor_list.push_back(std::make_shared<DescriptorBase>(descriptor.uuid));
            }
            characteristic_list.push_back(std::make_shared<CharacteristicBase>(
                characteristic.uuid, descriptor_list, characteristic.can_read, characteristic.can_write_request,
                characteristic.can_write_command, characteristic.can_notify, characteristic.can_indicate));
        }
        service_list.push_back(std::make_shared<ServiceBase>(service.uuid, characteristic_list));
    }

    return service_list;
}

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
    if (_conn_handle != BLE_CONN_HANDLE_INVALID) {
        auto response = _serial_protocol->simpleble_disconnect(_conn_handle);
        if (response.ret_code != 0) {
            fmt::println("Failed to disconnect during connect attempt: {}", response.ret_code);
        }

        // Wait for the disconnection to be confirmed.
        std::unique_lock<std::mutex> lock(disconnection_mutex_);
        disconnection_cv_.wait_for(lock, 500ms, [this]() { return !is_connected(); });
    }

    fmt::println("Disconnection confirmed");
    _conn_handle = BLE_CONN_HANDLE_INVALID;


    auto response = _serial_protocol->simpleble_connect(static_cast<simpleble_BluetoothAddressType>(_address_type),
                                                        _address);
    if (response.ret_code != 0) {
        throw Exception::OperationFailed(fmt::format("Error when attempting to connect: {}", response.ret_code));
    }

    // NOTE: Bluetooth connections are non-acknowledged by the peripheral. The connected event that we get
    //       is just the confirmation that the connection packet was sent, not that it was received.
    //       Our only option is to wait a bit after the connection event is received to see if maybe a
    //       disconnection event arises too, in which case the connection failed.

    // Wait for the connection to be confirmed.
    {
        std::unique_lock<std::mutex> lock(connection_mutex_);
        connection_cv_.wait_for(lock, 5000ms, [this]() { return _conn_handle != BLE_CONN_HANDLE_INVALID; });
        if (_conn_handle == BLE_CONN_HANDLE_INVALID) {
            //SIMPLEBLE_LOG_ERROR("Timeout while waiting for connection confirmation");
            fmt::println("Timeout while waiting for connection confirmation");
            return false;
        }
    }

    fmt::println("Connection confirmed");

    // Wait for the attributes to be discovered.
    {
        std::unique_lock<std::mutex> lock(attributes_discovered_mutex_);
        attributes_discovered_cv_.wait_for(lock, 15000ms, [this]() { return !_services.empty() || _conn_handle == BLE_CONN_HANDLE_INVALID; });
        if (_services.empty()) {
            fmt::println("Timeout while waiting for attributes to be discovered");
            return false;
        }

        if (_conn_handle == BLE_CONN_HANDLE_INVALID) {
            fmt::println("Connection lost during attribute discovery");
            return false;
        }
    }

    fmt::println("Attributes discovered. {} services found", _services.size());

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
    attributes_discovered_cv_.notify_all();

    // TODO: Only throw the callback if the disconection was unexpected.
    // SAFE_CALLBACK_CALL(this->_callback_on_disconnected);
}

void PeripheralDongl::notify_attribute_found(simpleble_AttributeFoundEvt const& attribute_found_evt) {
    for (size_t i = 0; i < attribute_found_evt.attributes_count; i++) {
        simpleble_Attribute const& attr = attribute_found_evt.attributes[i];
        switch (attr.which_attribute) {
            case simpleble_Attribute_service_tag: {
                BluetoothUUID uuid = _uuid_from_proto(attr.attribute.service.uuid);
                _services[attr.attribute.service.start_handle] = ServiceDefinition{
                    .uuid = uuid,
                    .start_handle = attr.attribute.service.start_handle,
                    .end_handle = attr.attribute.service.end_handle,
                };
                break;
            }
            case simpleble_Attribute_characteristic_tag: {
                uint16_t service_handle = 0;
                for (auto& service : _services) {
                    if (attr.attribute.characteristic.handle_decl >= service.second.start_handle &&
                        attr.attribute.characteristic.handle_decl <= service.second.end_handle) {
                        service_handle = service.first;
                        break;
                    }
                }

                if (service_handle == 0) {
                    break;
                }

                BluetoothUUID uuid = _uuid_from_proto(attr.attribute.characteristic.uuid);
                _services[service_handle].characteristics[attr.attribute.characteristic.handle_decl] =
                    CharacteristicDefinition{
                        .uuid = uuid,
                        .handle_decl = attr.attribute.characteristic.handle_decl,
                        .handle_value = attr.attribute.characteristic.handle_value,
                        .can_read = attr.attribute.characteristic.props.read,
                        .can_write_request = attr.attribute.characteristic.props.write,
                        .can_write_command = attr.attribute.characteristic.props.write_wo_resp,
                        .can_notify = attr.attribute.characteristic.props.notify,
                        .can_indicate = attr.attribute.characteristic.props.indicate,
                    };
                break;
            }
            case simpleble_Attribute_descriptor_tag: {
                // TODO: Identify the characteristic that this descriptor belongs to based on handle ranges.
                uint16_t service_handle = 0;
                for (auto& service : _services) {
                    if (attr.attribute.descriptor.handle >= service.second.start_handle &&
                        attr.attribute.descriptor.handle <= service.second.end_handle) {
                        service_handle = service.first;
                        break;
                    }
                }

                if (service_handle == 0) {
                    break;
                }

                uint16_t characteristic_handle = 0;
                for (auto& characteristic : _services[service_handle].characteristics) {
                    if (attr.attribute.descriptor.handle >= characteristic.second.handle_decl &&
                        attr.attribute.descriptor.handle <= characteristic.second.handle_value) {
                        characteristic_handle = characteristic.first;
                        break;
                    }
                }

                if (characteristic_handle == 0) {
                    break;
                }

                BluetoothUUID uuid = _uuid_from_proto(attr.attribute.descriptor.uuid);
                _services[service_handle]
                    .characteristics[characteristic_handle]
                    .descriptors[attr.attribute.descriptor.handle] = DescriptorDefinition{
                    .uuid = uuid,
                    .handle = attr.attribute.descriptor.handle,
                };
                break;
            }
        }
    }

    if (attribute_found_evt.is_last) {
        attributes_discovered_cv_.notify_all();
    }
}

BluetoothUUID PeripheralDongl::_uuid_from_proto(simpleble_UUID const& uuid) {
    switch (uuid.which_uuid) {
        case simpleble_UUID_uuid16_tag:
            return BluetoothUUID(fmt::format("0000{:04X}-0000-1000-8000-00805F9B34FB", uuid.uuid.uuid16.uuid));
        case simpleble_UUID_uuid32_tag:
            return BluetoothUUID(fmt::format("{:08X}-0000-1000-8000-00805F9B34FB", uuid.uuid.uuid32.uuid));
        case simpleble_UUID_uuid128_tag: {
            const auto& bytes = uuid.uuid.uuid128.uuid;
            std::string uuid_str;
            uuid_str.reserve(36);  // Pre-allocate memory for the UUID string

            uuid_str += fmt::format("{:02X}{:02X}{:02X}{:02X}", bytes[0], bytes[1], bytes[2], bytes[3]);
            uuid_str += "-";
            uuid_str += fmt::format("{:02X}{:02X}", bytes[4], bytes[5]);
            uuid_str += "-";
            uuid_str += fmt::format("{:02X}{:02X}", bytes[6], bytes[7]);
            uuid_str += "-";
            uuid_str += fmt::format("{:02X}{:02X}", bytes[8], bytes[9]);
            uuid_str += "-";
            uuid_str += fmt::format("{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}", bytes[10], bytes[11], bytes[12], bytes[13],
                                    bytes[14], bytes[15]);

            return BluetoothUUID(uuid_str);
        }
    }

    fmt::print("Unknown UUID type: {}\n", uuid.which_uuid);
    // Should not be reached
    return BluetoothUUID();
}