#include "Commands.h"
#include <fmt/format.h>
#include <typeinfo>

using namespace SimpleBLE::Dongl::CMD;


kvn::bytearray UartCommand::to_bytes() const {
    kvn::bytearray command;
    command.push_back(UART_START_BYTE);
    command.push_back(get_op_code());
    command.push_back(static_cast<uint16_t>(payload.size()));
    command.insert(command.end(), payload.begin(), payload.end());
    uint16_t crc = calculate_crc(command.data(), command.size());
    command.push_back(crc);
    return command;
}

std::string UartCommand::to_string() const {
    return fmt::format("{} (Payload: {})",
        typeid(*this).name(),
        payload.empty() ? "None" : payload.toHex());
}

UartSetUartConfigCommand::UartSetUartConfigCommand(uint32_t baudrate, bool hw_flow_ctrl)
    : baudrate(baudrate), hw_flow_ctrl(hw_flow_ctrl) {
    payload.push_back(baudrate);
    payload.push_back(static_cast<uint8_t>(hw_flow_ctrl));
}

std::string UartSetUartConfigCommand::to_string() const {
    return fmt::format("UartSetUartConfigCommand(Baudrate: {}, HW Flow Control: {})",
        baudrate, hw_flow_ctrl);
}

UartSetDeviceNameCommand::UartSetDeviceNameCommand(const std::string& name)
    : name(name) {
    payload.insert(payload.end(), name.begin(), name.end());
}

std::string UartSetDeviceNameCommand::to_string() const {
    return fmt::format("UartSetDeviceNameCommand(Device Name: {})", name);
}

UartSetDataConnectionParametersCommand::UartSetDataConnectionParametersCommand(
    uint32_t interval, std::optional<uint64_t> timeout, std::optional<uint32_t> event_length)
    : interval(interval), timeout(timeout), event_length(event_length) {
    payload.push_back(interval);
    if (timeout) payload.push_back(*timeout);
    if (event_length) payload.push_back(*event_length);
}

std::string UartSetDataConnectionParametersCommand::to_string() const {
    return fmt::format("UartSetDataConnectionParametersCommand(Interval: {}, Timeout: {}, Event Length: {})",
        interval,
        timeout ? fmt::format("{}", *timeout) : "None",
        event_length ? fmt::format("{}", *event_length) : "None");
}

UartSetRealTimeConnectionParametersCommand::UartSetRealTimeConnectionParametersCommand(
    uint32_t interval, uint32_t latency, uint8_t num_of_retransmission, uint16_t num_of_payload_bytes)
    : interval(interval), latency(latency), num_of_retransmission(num_of_retransmission),
      num_of_payload_bytes(num_of_payload_bytes) {
    payload.push_back(interval);
    payload.push_back(latency);
    payload.push_back(num_of_retransmission);
    payload.push_back(num_of_payload_bytes);
}

std::string UartSetRealTimeConnectionParametersCommand::to_string() const {
    return fmt::format("UartSetRealTimeConnectionParametersCommand(Interval: {}, Latency: {}, Num of Retransmission: {}, Num of Payload Bytes: {})",
        interval, latency, num_of_retransmission, num_of_payload_bytes);
}

UartSetSecurityModeCommand::UartSetSecurityModeCommand(SecurityMode mode, std::optional<uint32_t> passkey)
    : mode(mode), passkey(passkey) {
    payload.push_back(static_cast<uint8_t>(mode));
    if (passkey) payload.push_back(*passkey);
}

std::string UartSetSecurityModeCommand::to_string() const {
    return fmt::format("UartSetSecurityModeCommand(Mode: {}, Passkey: {})",
        static_cast<int>(mode),
        passkey ? fmt::format("{}", *passkey) : "None");
}

UartReadDataConnectionStatusCommand::UartReadDataConnectionStatusCommand(uint8_t peer_handle)
    : peer_handle(peer_handle) {
    payload.push_back(peer_handle);
}

std::string UartReadDataConnectionStatusCommand::to_string() const {
    return fmt::format("UartReadDataConnectionStatusCommand(Peer Handle: {})", peer_handle);
}

UartConnectDataConnectionCommand::UartConnectDataConnectionCommand(uint8_t peer_handle,
                                                                 const kvn::bytearray& peer_address)
    : peer_handle(peer_handle), peer_address(peer_address) {
    payload.push_back(peer_handle);
    payload.insert(payload.end(), peer_address.begin(), peer_address.end());
}

std::string UartConnectDataConnectionCommand::to_string() const {
    return fmt::format("UartConnectDataConnectionCommand(Peer Handle: {}, Peer Address: {})",
        peer_handle, peer_address.toHex());
}

UartDisconnectDataConnectionCommand::UartDisconnectDataConnectionCommand(uint8_t peer_handle)
    : peer_handle(peer_handle) {
    payload.push_back(peer_handle);
}

std::string UartDisconnectDataConnectionCommand::to_string() const {
    return fmt::format("UartDisconnectDataConnectionCommand(Peer Handle: {})", peer_handle);
}

UartReadRealTimeConnectionStatusCommand::UartReadRealTimeConnectionStatusCommand(uint8_t peer_handle)
    : peer_handle(peer_handle) {
    payload.push_back(peer_handle);
}

std::string UartReadRealTimeConnectionStatusCommand::to_string() const {
    return fmt::format("UartReadRealTimeConnectionStatusCommand(Peer Handle: {})", peer_handle);
}

UartConnectRealTimeConnectionCommand::UartConnectRealTimeConnectionCommand(uint8_t peer_handle)
    : peer_handle(peer_handle) {
    payload.push_back(peer_handle);
}

std::string UartConnectRealTimeConnectionCommand::to_string() const {
    return fmt::format("UartConnectRealTimeConnectionCommand(Peer Handle: {})", peer_handle);
}

UartDisconnectRealTimeConnectionCommand::UartDisconnectRealTimeConnectionCommand(uint8_t peer_handle)
    : peer_handle(peer_handle) {
    payload.push_back(peer_handle);
}

std::string UartDisconnectRealTimeConnectionCommand::to_string() const {
    return fmt::format("UartDisconnectRealTimeConnectionCommand(Peer Handle: {})", peer_handle);
}

UartTxDataCommand::UartTxDataCommand(uint8_t peer_handle, const kvn::bytearray& data)
    : peer_handle(peer_handle), data(data) {
    payload.push_back(peer_handle);
    payload.insert(payload.end(), data.begin(), data.end());
}

std::string UartTxDataCommand::to_string() const {
    return fmt::format("UartTxDataCommand(Peer Handle: {}, Data: {})",
        peer_handle, data.toHex());
}

UartPrepareFwUpdateCommand::UartPrepareFwUpdateCommand(const kvn::bytearray& hash, uint32_t size)
    : hash(hash), size(size) {
    payload.insert(payload.end(), hash.begin(), hash.end());
    payload.push_back(size);
}

std::string UartPrepareFwUpdateCommand::to_string() const {
    return fmt::format("UartPrepareFwUpdateCommand(Hash: {}, Size: {})",
        hash.toHex(), size);
}

UartWriteFwDataCommand::UartWriteFwDataCommand(uint16_t seq, const kvn::bytearray& data)
    : seq(seq), data(data) {
    payload.push_back(seq);
    payload.insert(payload.end(), data.begin(), data.end());
}

std::string UartWriteFwDataCommand::to_string() const {
    return fmt::format("UartWriteFwDataCommand(Seq: {}, Data: {})",
        seq, data.toHex());
}

UartEchoCommand::UartEchoCommand(const kvn::bytearray& data)
    : data(data) {
    payload = data;
}

std::string UartEchoCommand::to_string() const {
    return fmt::format("UartEchoCommand(Data: {})", data.toHex());
}