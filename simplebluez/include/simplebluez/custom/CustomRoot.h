#pragma once

#include <simpledbus/advanced/Proxy.h>
#include <simpledbus/interfaces/ObjectManager.h>
#include <simplebluez/custom/Agent.h>

namespace SimpleBluez {

class CustomRoot : public SimpleDBus::Proxy {
  public:
    CustomRoot(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path);
    virtual ~CustomRoot() = default;

    std::shared_ptr<Agent> get_agent();

    // ----- INTERNAL CALLBACKS -----
    void on_registration() override;

  private:
    std::shared_ptr<Agent> _agent;
    std::shared_ptr<SimpleDBus::Interfaces::ObjectManager> object_manager();
};

}  // namespace SimpleBluez
