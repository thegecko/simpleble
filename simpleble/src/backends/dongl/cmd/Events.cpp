#include "Events.h"
#include <fmt/format.h>
#include <array>
#include <memory>
#include <typeinfo>

namespace SimpleBLE {
namespace Dongl {
namespace CMD {

static bool is_bit_set(uint8_t bitfield, uint8_t bit_index) { return (bitfield >> bit_index) & 1; }

static void verify_checksum(const kvn::bytearray& raw_bytes) {
    if (raw_bytes.size() < 2) {
        throw UartEventCrcException(raw_bytes, kvn::bytearray(), kvn::bytearray());
    }
    kvn::bytearray crc_rx(raw_bytes.end() - 2, raw_bytes.end());
    uint16_t crc_rx_value = *reinterpret_cast<const uint16_t*>(crc_rx.data());

    uint16_t crc_calc = calculate_crc(raw_bytes.data(), raw_bytes.size() - 2);
    kvn::bytearray crc_calc_bytes = {static_cast<uint8_t>(crc_calc & 0xFF), static_cast<uint8_t>(crc_calc >> 8)};
    if (crc_rx_value != crc_calc) {
        throw UartEventCrcException(raw_bytes, crc_rx, crc_calc_bytes);
    }
}

static const std::map<uint8_t, std::function<std::unique_ptr<UartEvent>(const kvn::bytearray&)>> EVENT_MAPPING = {
    {UartStatusEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartStatusEvent>(payload); }},
    {UartVersionEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartVersionEvent>(payload); }},
    {UartTransmitBufferEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartTransmitBufferEvent>(payload); }},
    {UartDeviceIdEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartDeviceIdEvent>(payload); }},
    {UartConfigEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartConfigEvent>(payload); }},
    {UartDeviceNameEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartDeviceNameEvent>(payload); }},
    {UartDataConnectionParametersEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartDataConnectionParametersEvent>(payload); }},
    {UartRealTimeConnectionParametersEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartRealTimeConnectionParametersEvent>(payload); }},
    {UartSecurityModeEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartSecurityModeEvent>(payload); }},
    {UartScanResultEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartScanResultEvent>(payload); }},
    {UartDataConnectionStatusEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartDataConnectionStatusEvent>(payload); }},
    {UartRealTimeConnectionStatusEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartRealTimeConnectionStatusEvent>(payload); }},
    {UartRxDataEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartRxDataEvent>(payload); }},
    {UartFwInfoEvent::op_code,
     [](const kvn::bytearray& payload) { return std::make_unique<UartFwInfoEvent>(payload); }},
    {UartEchoEvent::op_code, [](const kvn::bytearray& payload) { return std::make_unique<UartEchoEvent>(payload); }}};

UartEventStartByteException::UartEventStartByteException(const kvn::bytearray& raw_bytes) : raw_bytes(raw_bytes) {}

std::string UartEventStartByteException::what() const noexcept {
    return fmt::format("UartEventStartByteException with start byte 0x{:02X}.", raw_bytes.empty() ? 0 : raw_bytes[0]);
}

UartEventCrcException::UartEventCrcException(const kvn::bytearray& raw_bytes, const kvn::bytearray& crc_rx,
                                             const kvn::bytearray& crc_calc)
    : raw_bytes(raw_bytes), crc_rx(crc_rx), crc_calc(crc_calc) {}

std::string UartEventCrcException::what() const noexcept {
    return fmt::format("UartEventCrcException received CRC {} but should be {}.", crc_rx.toHex(), crc_calc.toHex());
}

UartEventPayloadException::UartEventPayloadException(uint8_t op_code, const kvn::bytearray& payload_bytes)
    : op_code(op_code), payload_bytes(payload_bytes) {}

std::string UartEventPayloadException::what() const noexcept {
    return fmt::format("UartEventPayloadException with op_code: 0x{:02X} and payload length: {} bytes.", op_code,
                       payload_bytes.size());
}

std::unique_ptr<UartEvent> UartEvent::from_bytes(const kvn::bytearray& raw_bytes) {
    if (raw_bytes.empty() || raw_bytes[0] != UART_START_BYTE) {
        throw UartEventStartByteException(raw_bytes);
    }
    verify_checksum(raw_bytes);
    uint8_t op_code = raw_bytes[1];
    kvn::bytearray payload(raw_bytes.begin() + 4, raw_bytes.end() - 2);
    auto it = EVENT_MAPPING.find(op_code);
    if (it == EVENT_MAPPING.end()) {
        throw UartEventPayloadException(op_code, payload);
    }
    return it->second(payload);
}

std::string UartEvent::to_string() const {
    return fmt::format("{} (Payload: {})", typeid(*this).name(), payload.empty() ? "None" : payload.toHex());
}

UartStatusEvent::UartStatusEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() != EventPayloadLength::STATUS_EVENT_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    status_op_code = payload_bytes[0];
    status = static_cast<Status>(payload_bytes[1]);
}

std::string UartStatusEvent::to_string() const {
    return fmt::format("UartStatusEvent(Command: 0x{:02X}, Status: {})", status_op_code, static_cast<int>(status));
}

UartVersionEvent::UartVersionEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() != EventPayloadLength::VERSION_EVENT_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    hw_version = payload_bytes[0];
    major_sw_version = payload_bytes[1];
    minor_sw_version = payload_bytes[2];
    bugfix_version = payload_bytes[3];
}

std::string UartVersionEvent::to_string() const {
    return fmt::format("UartVersionEvent(HW: {}, SW: {}.{}.{})", hw_version, major_sw_version, minor_sw_version,
                       bugfix_version);
}

UartTransmitBufferEvent::UartTransmitBufferEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() != EventPayloadLength::TRANSMIT_BUFFER_EVENT_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    num_available_transmit_buffers = payload_bytes[0];
}

std::string UartTransmitBufferEvent::to_string() const {
    return fmt::format("UartTransmitBufferEvent(Transmit Buffers: {})", num_available_transmit_buffers);
}

UartDeviceIdEvent::UartDeviceIdEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() != EventPayloadLength::DEVICE_ID_EVENT_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    device_id = payload_bytes;
}

std::string UartDeviceIdEvent::to_string() const {
    return fmt::format("UartDeviceIdEvent(Device ID: {})", device_id.toHex());
}

UartConfigEvent::UartConfigEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() != EventPayloadLength::UART_CONFIG_EVENT_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    baud_rate = *reinterpret_cast<const uint32_t*>(payload_bytes.data());
    flow_control = payload_bytes[4];
}

std::string UartConfigEvent::to_string() const {
    return fmt::format("UartConfigEvent(Baudrate: {}, Flow Control: {})", baud_rate, flow_control);
}

UartDeviceNameEvent::UartDeviceNameEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() > EventPayloadLength::DEVICE_NAME_EVENT_PAYLOAD_LENGTH || payload_bytes.empty()) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    device_name = std::string(payload_bytes.begin(), payload_bytes.end());
}

std::string UartDeviceNameEvent::to_string() const {
    return fmt::format("UartDeviceNameEvent(Device Name: {})", device_name);
}

UartDataConnectionParametersEvent::UartDataConnectionParametersEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() != EventPayloadLength::DATA_CONNECTION_PARAMETERS_EVENT_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    connection_interval_us = *reinterpret_cast<const uint32_t*>(payload_bytes.data());
    supervision_timeout_us = *reinterpret_cast<const uint64_t*>(payload_bytes.data() + 4);
    connection_event_length_us = *reinterpret_cast<const uint32_t*>(payload_bytes.data() + 12);
}

std::string UartDataConnectionParametersEvent::to_string() const {
    return fmt::format("UartDataConnectionParametersEvent(Interval: {}, Timeout: {}, Event Length: {})",
                       connection_interval_us, supervision_timeout_us, connection_event_length_us);
}

UartRealTimeConnectionParametersEvent::UartRealTimeConnectionParametersEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() != EventPayloadLength::REAL_TIME_CONNECTION_PARAMETERS_EVENT_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    interval = *reinterpret_cast<const uint32_t*>(payload_bytes.data());
    latency = *reinterpret_cast<const uint32_t*>(payload_bytes.data() + 4);
    num_of_retransmission = payload_bytes[8];
    num_of_payload_bytes = *reinterpret_cast<const uint16_t*>(payload_bytes.data() + 9);
}

std::string UartRealTimeConnectionParametersEvent::to_string() const {
    return fmt::format(
        "UartRealTimeConnectionParametersEvent(Interval: {}, Latency: {}, Number of Retransmission: {}, Number of "
        "Payload Bytes: {})",
        interval, latency, num_of_retransmission, num_of_payload_bytes);
}

UartSecurityModeEvent::UartSecurityModeEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() != EventPayloadLength::SECURITY_MODE_EVENT_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    security_mode = static_cast<SecurityMode>(payload_bytes[0]);
}

std::string UartSecurityModeEvent::to_string() const {
    return fmt::format("UartSecurityModeEvent(Mode: {})", static_cast<int>(security_mode));
}

UartScanResultEvent::UartScanResultEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() < EventPayloadLength::SCAN_RESULT_EVENT_MINIMUM_PAYLOAD_LENGTH ||
        payload_bytes.size() > EventPayloadLength::SCAN_RESULT_EVENT_MAXIMUM_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    address = kvn::bytearray(payload_bytes.begin(), payload_bytes.begin() + 6);
    rssi = static_cast<int8_t>(payload_bytes[6]);
    name = std::string(payload_bytes.begin() + 7, payload_bytes.end());
}

std::string UartScanResultEvent::to_string() const {
    return fmt::format("UartScanResultEvent(Address: {}, Name: {}, RSSI: {})", address.toHex(), name, rssi);
}

UartDataConnectionStatusEvent::UartDataConnectionStatusEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() != EventPayloadLength::DATA_CONNECTION_STATUS_EVENT_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    peer_handle = payload_bytes[0];
    connection_status_code = payload_bytes[1];
    is_established = is_bit_set(connection_status_code, 0);
    is_ready_transmit = is_bit_set(connection_status_code, 1);
    is_ready_receive = is_bit_set(connection_status_code, 2);
    is_encrypted = is_bit_set(connection_status_code, 3);
    is_authenticated = is_bit_set(connection_status_code, 4);
    additional_status_info = static_cast<ConnectionStatus>(payload_bytes[2]);
    connection_interval_us = *reinterpret_cast<const uint32_t*>(payload_bytes.data() + 3);
    peripheral_latency = *reinterpret_cast<const uint16_t*>(payload_bytes.data() + 7);
    supervision_timeout_us = *reinterpret_cast<const uint64_t*>(payload_bytes.data() + 9);
    phy_mode = payload_bytes[17];
}

std::string UartDataConnectionStatusEvent::to_string() const {
    return fmt::format(
        "UartDataConnectionStatusEvent(Peer Handle: {}, Is Established: {}, Is Ready Transmit: {}, Is Ready Receive: "
        "{}, Is Encrypted: {}, Is Authenticated: {}, Additional Status Info: {}, Connection Interval: {} us, "
        "Peripheral Latency: {}, Supervision Timeout: {} us, PHY Mode: {})",
        peer_handle, is_established, is_ready_transmit, is_ready_receive, is_encrypted, is_authenticated,
        static_cast<int>(additional_status_info), connection_interval_us, peripheral_latency, supervision_timeout_us,
        phy_mode);
}

UartRealTimeConnectionStatusEvent::UartRealTimeConnectionStatusEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() != EventPayloadLength::REAL_TIME_CONNECTION_STATUS_EVENT_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    peer_handle = payload_bytes[0];
    connection_status_code = payload_bytes[1];
    is_established = is_bit_set(connection_status_code, 0);
    is_ready_transmit = is_bit_set(connection_status_code, 1);
    is_ready_receive = is_bit_set(connection_status_code, 2);
    is_encrypted = is_bit_set(connection_status_code, 3);
    is_authenticated = is_bit_set(connection_status_code, 4);
    additional_status_info = static_cast<ConnectionStatus>(payload_bytes[2]);
}

std::string UartRealTimeConnectionStatusEvent::to_string() const {
    return fmt::format(
        "UartRealTimeConnectionStatusEvent(Peer Handle: {}, Is Established: {}, Is Ready Transmit: {}, Is Ready "
        "Receive: {}, Is Encrypted: {}, Is Authenticated: {}, Additional Status Info: {})",
        peer_handle, is_established, is_ready_transmit, is_ready_receive, is_encrypted, is_authenticated,
        static_cast<int>(additional_status_info));
}

UartRxDataEvent::UartRxDataEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() > EventPayloadLength::RX_DATA_EVENT_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    peer_handle = payload_bytes[0];
    data = kvn::bytearray(payload_bytes.begin() + 1, payload_bytes.end());
}

std::string UartRxDataEvent::to_string() const {
    return fmt::format("UartRxDataEvent(Peer Handle: {}, Data: {})", peer_handle, data.toHex());
}

UartFwInfoEvent::UartFwInfoEvent(const kvn::bytearray& payload_bytes) {
    bool valid_length = false;
    for (size_t len : EventPayloadLength::FW_INFO_PAYLOAD_LENGTHS) {
        if (payload_bytes.size() == len) {
            valid_length = true;
            break;
        }
    }
    if (!valid_length) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    image_count = payload_bytes[0];
    image_1_slot = payload_bytes[1];
    image_1_version = {payload_bytes[2], payload_bytes[3], payload_bytes[4], payload_bytes[5]};
    image_1_flags = payload_bytes[6];
    image_1_flag_confirmed = is_bit_set(image_1_flags, 0);
    image_1_flag_pending = is_bit_set(image_1_flags, 1);
    image_1_hash = kvn::bytearray(payload_bytes.begin() + 7, payload_bytes.begin() + 39);
    if (image_count == 2) {
        image_2_slot = payload_bytes[39];
        image_2_version = {payload_bytes[40], payload_bytes[41], payload_bytes[42], payload_bytes[43]};
        image_2_flags = payload_bytes[44];
        image_2_flag_confirmed = is_bit_set(image_2_flags, 0);
        image_2_flag_pending = is_bit_set(image_2_flags, 1);
        image_2_hash = kvn::bytearray(payload_bytes.begin() + 45, payload_bytes.begin() + 77);
    }
}

std::string UartFwInfoEvent::to_string() const {
    std::string image_2_str = image_count == 2 ? fmt::format("{}.{}.{}.{}", image_2_version[0], image_2_version[1],
                                                             image_2_version[2], image_2_version[3])
                                               : "N/A";
    return fmt::format("UartFwInfoEvent(Images: {}, Image 1: {}.{}.{}.{}, Image 2: {})", image_count,
                       image_1_version[0], image_1_version[1], image_1_version[2], image_1_version[3], image_2_str);
}

UartEchoEvent::UartEchoEvent(const kvn::bytearray& payload_bytes) {
    if (payload_bytes.size() > EventPayloadLength::ECHO_EVENT_MAXIMUM_PAYLOAD_LENGTH) {
        throw UartEventPayloadException(op_code, payload_bytes);
    }
    payload = payload_bytes;
    data = payload_bytes;
}

std::string UartEchoEvent::to_string() const { return fmt::format("UartEchoEvent(Data: {})", data.toHex()); }

}  // namespace CMD
}  // namespace Dongl
}  // namespace SimpleBLE