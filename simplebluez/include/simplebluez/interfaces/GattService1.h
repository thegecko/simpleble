#pragma once

#include <simpledbus/advanced/Interface.h>
#include <simpledbus/advanced/InterfaceRegistry.h>
#include <string>
#include "simpledbus/base/HolderUtils.h"

namespace SimpleBluez {

class GattService1 : public SimpleDBus::Interface {
  public:
    GattService1(std::shared_ptr<SimpleDBus::Connection> conn, std::shared_ptr<SimpleDBus::Proxy> proxy);
    virtual ~GattService1();

    // ----- METHODS -----

    // ----- PROPERTIES -----
    Property<std::string>& UUID = create_property<std::string>("UUID");
    Property<bool>& Primary = create_property<bool>("Primary");
    CustomProperty<std::vector<std::string>>& Characteristics = create_custom_property<std::vector<std::string>>(
        "Characteristics", SimpleDBus::HolderUtils::from_string_array, SimpleDBus::HolderUtils::to_string_array);

  private:
    static const SimpleDBus::AutoRegisterInterface<GattService1> registry;
};

}  // namespace SimpleBluez
