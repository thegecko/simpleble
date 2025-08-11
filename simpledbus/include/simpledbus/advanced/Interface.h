#pragma once

#include <simpledbus/base/Connection.h>

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <string>

namespace SimpleDBus {

class Proxy;

class Interface {
  public:
    Interface(std::shared_ptr<Connection> conn, std::shared_ptr<Proxy> proxy, const std::string& interface_name);

    virtual ~Interface() = default;

    // ----- LIFE CYCLE -----
    void load(Holder options);
    void unload();
    bool is_loaded() const;

    // ----- METHODS -----
    Message create_method_call(const std::string& method_name);

    // ----- PROPERTIES -----
    virtual void property_changed(std::string option_name);


    // ! TODO: We need to figure out a good architecture to let any generic interface access the Properties object of its Proxy.
    void property_refresh(const std::string& property_name);

    // ----- SIGNALS -----
    void signal_property_changed(Holder changed_properties, Holder invalidated_properties);

    // ----- MESSAGES -----
    virtual void message_handle(Message& msg);

    // ! The following properties are set as public to allow access to the Properties interface.
    std::recursive_mutex _property_update_mutex;
    std::map<std::string, bool> _property_valid_map;
    std::map<std::string, Holder> _properties;

  protected:
    std::atomic_bool _loaded{true};

    std::string _path;
    std::string _bus_name;
    std::string _interface_name;
    std::shared_ptr<Connection> _conn;
    std::weak_ptr<Proxy> _proxy;

    std::shared_ptr<Proxy> proxy() const;
};

}  // namespace SimpleDBus
