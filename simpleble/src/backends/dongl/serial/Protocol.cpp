#include "Protocol.h"

using namespace SimpleBLE::Dongl::Serial;

#include "nanopb/pb_decode.h"
#include "nanopb/pb_encode.h"

Protocol::Protocol(const std::string& device_path) : _wire(std::make_unique<Wire>(device_path)) {
    // Set up the Wire packet callback to handle incoming packets
    _wire->set_packet_callback([this](const std::vector<uint8_t>& packet) {
        if (!_response_callback && !_event_callback) {
            return;
        }

        dongl_D2H d2h;
        pb_istream_t stream = pb_istream_from_buffer(packet.data(), packet.size());
        if (!pb_decode(&stream, dongl_D2H_fields, &d2h)) {
            // TODO: Handle decoding failure
            return;
        }

        if (d2h.which_type == dongl_D2H_rsp_tag) {
            _response_callback(d2h.type.rsp);
        } else if (d2h.which_type == dongl_D2H_evt_tag) {
            _event_callback(d2h.type.evt);
        }
    });

    // Set up the Wire error callback
    _wire->set_error_callback([this](Wire::Error error) {
        // TODO: Convert Wire errors to protocol errors
        if (_error_callback) {
            _error_callback(error);
        }
    });
}

Protocol::~Protocol() {}

void Protocol::send_command(const dongl_Command& command) {
    size_t command_size = 0;
    pb_get_encoded_size(&command_size, dongl_Command_fields, &command);

    std::vector<uint8_t> tx_buffer_raw(command_size);
    pb_ostream_t stream = pb_ostream_from_buffer(tx_buffer_raw.data(), tx_buffer_raw.size());
    bool status = pb_encode(&stream, dongl_Command_fields, &command);
    if (!status) {
        // TODO: Handle encoding failure
        return;
    }

    _wire->send_packet(tx_buffer_raw);
}

void Protocol::set_response_callback(ResponseCallback callback) {
    _response_callback = std::move(callback);
}

void Protocol::set_event_callback(EventCallback callback) {
    _event_callback = std::move(callback);
}

void Protocol::set_error_callback(ErrorCallback callback) {
    _error_callback = std::move(callback);
}
