#include <simplebluez/custom/CustomRoot.h>

using namespace SimpleBluez;

CustomRoot::CustomRoot(std::shared_ptr<SimpleDBus::Connection> conn, const std::string& bus_name, const std::string& path)
    : Proxy(conn, bus_name, path) {}

void CustomRoot::on_registration() {
    _interfaces.emplace(std::make_pair("org.freedesktop.DBus.ObjectManager", std::make_shared<SimpleDBus::Interfaces::ObjectManager>(_conn, shared_from_this())));

    // Create the agent that will handle pairing.
    _agent = Proxy::create<Agent>(_conn, "org.bluez", "/agent");
    path_append_child("/agent", std::static_pointer_cast<SimpleDBus::Proxy>(_agent));
}

std::shared_ptr<Agent> CustomRoot::get_agent() { return std::dynamic_pointer_cast<Agent>(path_get("/agent")); }

std::shared_ptr<SimpleDBus::Interfaces::ObjectManager> CustomRoot::object_manager() {
    return std::dynamic_pointer_cast<SimpleDBus::Interfaces::ObjectManager>(interface_get("org.freedesktop.DBus.ObjectManager"));
}
