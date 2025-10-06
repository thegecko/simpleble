#pragma once
#include <kvn/kvn_bytearray.h>
#include <cstdint>
#include <optional>
#include <string>
#include <map>
#include "Commands.h"

namespace SimpleBLE {
namespace Dongl {
namespace CMD {

enum class Status : uint8_t {
    OK = 0x00,
    ERR_FAILED = 0xFF,
    ERR_INVALID_OP = 0xFE,
    ERR_INVALID_LEN = 0xFD,
    ERR_INVALID_PARAM = 0xFC,
    ERR_INVALID_STATE = 0xFB,
    ERR_NOT_READY = 0xF9,
    ERR_ALREADY = 0xF6,
    ERR_UART = 0xF3
};

enum class ConnectionStatus : uint8_t {
    OK = 0x00,
    UNKNOWN_CONNECTION_IDENTIFIER = 0x02,
    CONNECTION_TIMEOUT = 0x08,
    CONNECTION_REJECTED_DUE_TO_SECURITY_REASONS = 0x0E,
    REMOTE_USER_TERMINATED_CONNECTION = 0x13,
    CONNECTION_TERMINATED_BY_LOCAL_HOST = 0x16,
    INSUFFICIENT_SECURITY = 0x2F,
    CONNECTION_FAILED_TO_BE_ESTABLISHED = 0x3E
};

struct EventPayloadLength {
    static constexpr size_t NO_PAYLOAD = 0;
    static constexpr size_t STATUS_EVENT_PAYLOAD_LENGTH = 2;
    static constexpr size_t VERSION_EVENT_PAYLOAD_LENGTH = 4;
    static constexpr size_t DEVICE_ID_EVENT_PAYLOAD_LENGTH = 6;
    static constexpr size_t TRANSMIT_BUFFER_EVENT_PAYLOAD_LENGTH = 1;
    static constexpr size_t UART_CONFIG_EVENT_PAYLOAD_LENGTH = 5;
    static constexpr size_t DEVICE_NAME_EVENT_PAYLOAD_LENGTH = 8;
    static constexpr size_t DATA_CONNECTION_PARAMETERS_EVENT_PAYLOAD_LENGTH = 16;
    static constexpr size_t REAL_TIME_CONNECTION_PARAMETERS_EVENT_PAYLOAD_LENGTH = 11;
    static constexpr size_t SCAN_RESULT_EVENT_MINIMUM_PAYLOAD_LENGTH = 7;
    static constexpr size_t SCAN_RESULT_EVENT_MAXIMUM_PAYLOAD_LENGTH = 15;
    static constexpr size_t DATA_CONNECTION_STATUS_EVENT_PAYLOAD_LENGTH = 18;
    static constexpr size_t REAL_TIME_CONNECTION_STATUS_EVENT_PAYLOAD_LENGTH = 3;
    static constexpr size_t SECURITY_MODE_EVENT_PAYLOAD_LENGTH = 1;
    static constexpr size_t RX_DATA_EVENT_PAYLOAD_LENGTH = 513;
    static constexpr size_t ECHO_EVENT_MAXIMUM_PAYLOAD_LENGTH = 513;
    static constexpr size_t FW_INFO_PAYLOAD_LENGTHS[2] = {39, 77};
};

class UartEventException {
public:
    virtual ~UartEventException() = default;
};

class UartEventStartByteException : public UartEventException {
public:
    UartEventStartByteException(const kvn::bytearray& raw_bytes);
    std::string what() const noexcept;
private:
    kvn::bytearray raw_bytes;
};

class UartEventCrcException : public UartEventException {
public:
    UartEventCrcException(const kvn::bytearray& raw_bytes, const kvn::bytearray& crc_rx, const kvn::bytearray& crc_calc);
    std::string what() const noexcept;
private:
    kvn::bytearray raw_bytes;
    kvn::bytearray crc_rx;
    kvn::bytearray crc_calc;
};

class UartEventPayloadException : public UartEventException {
public:
    UartEventPayloadException(uint8_t op_code, const kvn::bytearray& payload_bytes);
    std::string what() const noexcept;
private:
    uint8_t op_code;
    kvn::bytearray payload_bytes;
};

class UartEvent {
protected:
    kvn::bytearray payload;
public:
    UartEvent() = default;
    virtual ~UartEvent() = default;
    static std::unique_ptr<UartEvent> from_bytes(const kvn::bytearray& raw_bytes);
    virtual std::string to_string() const;
};

class UartStatusEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0x80;
    UartStatusEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    uint8_t status_op_code;
    Status status;
};

class UartVersionEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0x81;
    UartVersionEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    uint8_t hw_version;
    uint8_t major_sw_version;
    uint8_t minor_sw_version;
    uint8_t bugfix_version;
};

class UartTransmitBufferEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0x82;
    UartTransmitBufferEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    uint8_t num_available_transmit_buffers;
};

class UartDeviceIdEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0x83;
    UartDeviceIdEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    kvn::bytearray device_id;
};

class UartConfigEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0x91;
    UartConfigEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    uint32_t baud_rate;
    uint8_t flow_control;
};

class UartDeviceNameEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0x93;
    UartDeviceNameEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    std::string device_name;
};

class UartDataConnectionParametersEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0x95;
    UartDataConnectionParametersEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    uint32_t connection_interval_us;
    uint64_t supervision_timeout_us;
    uint32_t connection_event_length_us;
};

class UartRealTimeConnectionParametersEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0x97;
    UartRealTimeConnectionParametersEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    uint32_t interval;
    uint32_t latency;
    uint8_t num_of_retransmission;
    uint16_t num_of_payload_bytes;
};

class UartSecurityModeEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0x99;
    UartSecurityModeEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    SecurityMode security_mode;
};

class UartScanResultEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0xA0;
    UartScanResultEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    kvn::bytearray address;
    int8_t rssi;
    std::string name;
};

class UartDataConnectionStatusEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0xB0;
    UartDataConnectionStatusEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    uint8_t peer_handle;
    uint8_t connection_status_code;
    bool is_established;
    bool is_ready_transmit;
    bool is_ready_receive;
    bool is_encrypted;
    bool is_authenticated;
    ConnectionStatus additional_status_info;
    uint32_t connection_interval_us;
    uint16_t peripheral_latency;
    uint64_t supervision_timeout_us;
    uint8_t phy_mode;
};

class UartRealTimeConnectionStatusEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0xB3;
    UartRealTimeConnectionStatusEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    uint8_t peer_handle;
    uint8_t connection_status_code;
    bool is_established;
    bool is_ready_transmit;
    bool is_ready_receive;
    bool is_encrypted;
    bool is_authenticated;
    ConnectionStatus additional_status_info;
};

class UartRxDataEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0xB6;
    UartRxDataEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    uint8_t peer_handle;
    kvn::bytearray data;
};

class UartFwInfoEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0xC0;
    UartFwInfoEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    uint8_t image_count;
    uint8_t image_1_slot;
    std::array<uint8_t, 4> image_1_version;
    uint8_t image_1_flags;
    bool image_1_flag_confirmed;
    bool image_1_flag_pending;
    kvn::bytearray image_1_hash;
    uint8_t image_2_slot;
    std::array<uint8_t, 4> image_2_version;
    uint8_t image_2_flags;
    bool image_2_flag_confirmed;
    bool image_2_flag_pending;
    kvn::bytearray image_2_hash;
};

class UartEchoEvent : public UartEvent {
public:
    static constexpr uint8_t op_code = 0xF0;
    UartEchoEvent(const kvn::bytearray& payload_bytes);
    std::string to_string() const override;
private:
    kvn::bytearray data;
};

} // namespace CMD
} // namespace Dongl
} // namespace SimpleBLE