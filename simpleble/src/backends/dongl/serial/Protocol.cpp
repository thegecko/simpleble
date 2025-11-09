#include "Protocol.h"

using namespace SimpleBLE::Dongl::Serial;

Protocol::Protocol(const std::string& device_path) : ProtocolBase(device_path) {}

Protocol::~Protocol() {}
