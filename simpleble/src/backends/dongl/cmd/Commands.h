#pragma once

#include <kvn/kvn_bytearray.h>
#include <cstdint>
#include <optional>
#include <string>

namespace SimpleBLE {
namespace Dongl {
namespace CMD {

enum class SecurityMode : uint8_t { NONE = 0x00, PAIRING = 0x01, PAIRING_WITH_PASSKEY = 0x02 };

constexpr uint8_t UART_START_BYTE = 0x05;
constexpr size_t UART_FW_DATA_BLOCK_SIZE = 256;

// Forward declaration for CRC function
uint16_t calculate_crc(const uint8_t* src, size_t len);

class UartCommand {
  protected:
    kvn::bytearray payload;

  public:
    UartCommand() = default;
    virtual ~UartCommand() = default;

    virtual uint8_t get_op_code() const = 0;
    virtual kvn::bytearray to_bytes() const;
    virtual std::string to_string() const;
};

class UartRebootCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x00;
    uint8_t get_op_code() const override { return op_code; }
};

class UartReadVersionCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x01;
    uint8_t get_op_code() const override { return op_code; }
};

class UartReadTransmitBufferCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x02;
    uint8_t get_op_code() const override { return op_code; }
};

class UartReadDeviceIdCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x03;
    uint8_t get_op_code() const override { return op_code; }
};

class UartFactoryResetCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x0F;
    uint8_t get_op_code() const override { return op_code; }
};

class UartSetUartConfigCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x10;
    UartSetUartConfigCommand(uint32_t baudrate, bool hw_flow_ctrl);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    uint32_t baudrate;
    bool hw_flow_ctrl;
};

class UartReadUartConfigCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x11;
    uint8_t get_op_code() const override { return op_code; }
};

class UartSetDeviceNameCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x12;
    UartSetDeviceNameCommand(const std::string& name);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    std::string name;
};

class UartReadDeviceNameCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x13;
    uint8_t get_op_code() const override { return op_code; }
};

class UartSetDataConnectionParametersCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x14;
    UartSetDataConnectionParametersCommand(uint32_t interval, std::optional<uint64_t> timeout = std::nullopt,
                                           std::optional<uint32_t> event_length = std::nullopt);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    uint32_t interval;
    std::optional<uint64_t> timeout;
    std::optional<uint32_t> event_length;
};

class UartReadDataConnectionParametersCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x15;
    uint8_t get_op_code() const override { return op_code; }
};

class UartSetRealTimeConnectionParametersCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x16;
    UartSetRealTimeConnectionParametersCommand(uint32_t interval, uint32_t latency, uint8_t num_of_retransmission,
                                               uint16_t num_of_payload_bytes);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    uint32_t interval;
    uint32_t latency;
    uint8_t num_of_retransmission;
    uint16_t num_of_payload_bytes;
};

class UartReadRealTimeConnectionParametersCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x17;
    uint8_t get_op_code() const override { return op_code; }
};

class UartSetSecurityModeCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x18;
    UartSetSecurityModeCommand(SecurityMode mode, std::optional<uint32_t> passkey = std::nullopt);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    SecurityMode mode;
    std::optional<uint32_t> passkey;
};

class UartReadSecurityModeCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x19;
    uint8_t get_op_code() const override { return op_code; }
};

class UartStartScanningCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x20;
    uint8_t get_op_code() const override { return op_code; }
};

class UartStopScanningCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x21;
    uint8_t get_op_code() const override { return op_code; }
};

class UartStartAdvertisingCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x22;
    uint8_t get_op_code() const override { return op_code; }
};

class UartStopAdvertisingCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x23;
    uint8_t get_op_code() const override { return op_code; }
};

class UartReadDataConnectionStatusCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x30;
    UartReadDataConnectionStatusCommand(uint8_t peer_handle);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    uint8_t peer_handle;
};

class UartConnectDataConnectionCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x31;
    UartConnectDataConnectionCommand(uint8_t peer_handle, const kvn::bytearray& peer_address);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    uint8_t peer_handle;
    kvn::bytearray peer_address;
};

class UartDisconnectDataConnectionCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x32;
    UartDisconnectDataConnectionCommand(uint8_t peer_handle);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    uint8_t peer_handle;
};

class UartReadRealTimeConnectionStatusCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x33;
    UartReadRealTimeConnectionStatusCommand(uint8_t peer_handle);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    uint8_t peer_handle;
};

class UartConnectRealTimeConnectionCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x34;
    UartConnectRealTimeConnectionCommand(uint8_t peer_handle);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    uint8_t peer_handle;
};

class UartDisconnectRealTimeConnectionCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x35;
    UartDisconnectRealTimeConnectionCommand(uint8_t peer_handle);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    uint8_t peer_handle;
};

class UartTxDataCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x36;
    UartTxDataCommand(uint8_t peer_handle, const kvn::bytearray& data);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    uint8_t peer_handle;
    kvn::bytearray data;
};

class UartReadFwInfoCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x40;
    uint8_t get_op_code() const override { return op_code; }
};

class UartPrepareFwUpdateCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x41;
    UartPrepareFwUpdateCommand(const kvn::bytearray& hash, uint32_t size);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    kvn::bytearray hash;
    uint32_t size;
};

class UartWriteFwDataCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x42;
    UartWriteFwDataCommand(uint16_t seq, const kvn::bytearray& data);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    uint16_t seq;
    kvn::bytearray data;
};

class UartVerifyFwDataCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x43;
    uint8_t get_op_code() const override { return op_code; }
};

class UartSwapFwCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x44;
    uint8_t get_op_code() const override { return op_code; }
};

class UartConfirmFwCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x45;
    uint8_t get_op_code() const override { return op_code; }
};

class UartEchoCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0x70;
    UartEchoCommand(const kvn::bytearray& data);
    uint8_t get_op_code() const override { return op_code; }
    std::string to_string() const override;

  private:
    kvn::bytearray data;
};

class UartUnsupportedCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0xFE;
    uint8_t get_op_code() const override { return op_code; }
};

class UartInvalidCommand : public UartCommand {
  public:
    static constexpr uint8_t op_code = 0xFF;
    uint8_t get_op_code() const override { return op_code; }
};

}  // namespace CMD
}  // namespace Dongl
}  // namespace SimpleBLE