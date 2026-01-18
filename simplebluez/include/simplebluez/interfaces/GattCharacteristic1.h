#pragma once

#include <simpledbus/advanced/Interface.h>
#include <simpledbus/advanced/InterfaceRegistry.h>

#include "kvn/kvn_safe_callback.hpp"

#include <simplebluez/Types.h>

#include <string>

#include <simpledbus/base/HolderUtils.h>

namespace SimpleBluez {

class GattCharacteristic1 : public SimpleDBus::Interface {
  public:
    typedef enum { REQUEST = 0, COMMAND } WriteType;

    GattCharacteristic1(std::shared_ptr<SimpleDBus::Connection> conn, std::shared_ptr<SimpleDBus::Proxy> proxy);
    virtual ~GattCharacteristic1();

    // ----- METHODS -----
    void StartNotify();
    void StopNotify();
    void WriteValue(const ByteArray& value, WriteType type);
    ByteArray ReadValue();

    // ----- PROPERTIES -----
    Property<std::string>& UUID = create_property<std::string>("UUID");
    CustomProperty<ByteArray>& Value = create_custom_property<ByteArray>("Value", SimpleDBus::HolderUtils::from_byte_array,
                                                                         SimpleDBus::HolderUtils::to_byte_array);
    Property<bool>& Notifying = create_property<bool>("Notifying");
    CustomProperty<std::vector<std::string>>& Flags = create_custom_property<std::vector<std::string>>(
        "Flags", SimpleDBus::HolderUtils::from_string_array, SimpleDBus::HolderUtils::to_string_array);
    Property<uint16_t>& MTU = create_property<uint16_t>("MTU");

  private:
    static const SimpleDBus::AutoRegisterInterface<GattCharacteristic1> registry;
};

}  // namespace SimpleBluez
