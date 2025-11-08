#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Wire.h"

#include "protocol/d2h.pb.h"
#include "protocol/h2d.pb.h"

namespace SimpleBLE {
namespace Dongl {
namespace Serial {

class Protocol {
public:
    /**
     * @brief Response callback type.
     */
    using ResponseCallback = std::function<void(const dongl_Response&)>;

    /**
     * @brief Event callback type.
     */
    using EventCallback = std::function<void(const dongl_Event&)>;

    /**
     * @brief Error callback type.
     */
    using ErrorCallback = std::function<void(Wire::Error)>;

    Protocol(std::unique_ptr<Wire> wire);
    ~Protocol();

    /**
     * @brief Sends a command to the Dongl device.
     *
     * @param command The command to send.
     */
    void send_command(const dongl_Command& command);

    /**
     * @brief Sets the callback for received responses.
     *
     * @param callback Function to call when a response is received.
     */
    void set_response_callback(ResponseCallback callback);

    /**
     * @brief Sets the callback for received events.
     *
     * @param callback Function to call when an event is received.
     */
    void set_event_callback(EventCallback callback);

    /**
     * @brief Sets the callback for protocol errors.
     *
     * @param callback Function to call on protocol errors.
     */
    void set_error_callback(ErrorCallback callback);

private:
    std::unique_ptr<Wire> _wire;
    ResponseCallback _response_callback;
    EventCallback _event_callback;
    ErrorCallback _error_callback;
};

}  // namespace Serial
}  // namespace Dongl
}  // namespace SimpleBLE
