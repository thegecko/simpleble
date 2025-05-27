#pragma once
#include <chrono>

namespace SimpleBLE {
namespace Config {
    namespace SimpleBluez {
        inline static std::chrono::steady_clock::duration connection_timeout = std::chrono::seconds(2);
        inline static std::chrono::steady_clock::duration disconnection_timeout = std::chrono::seconds(1);

        static void reset() {
            connection_timeout = std::chrono::seconds(2);
            disconnection_timeout = std::chrono::seconds(1);
        }
    }

    namespace WinRT {
        inline static bool experimental_use_own_mta_apartment = true;

        static void reset() {
            experimental_use_own_mta_apartment = true;
        }
    }

    namespace CoreBluetooth {
        static void reset() {}
    }

    namespace Android {
        enum class ConnectionPriorityRequest {
            DISABLED = -1,
            BALANCED = 0,
            HIGH = 1,
            LOW_POWER = 2,
            DCK = 3
        };

        inline static ConnectionPriorityRequest connection_priority_request = ConnectionPriorityRequest::DISABLED;

        static void reset() {
            connection_priority_request = ConnectionPriorityRequest::DISABLED;
        }
    }

    namespace Base {
        static void reset_all() {
            SimpleBluez::reset();
            WinRT::reset();
            CoreBluetooth::reset();
            Android::reset();
        }
    }
}  // namespace Config
}  // namespace SimpleBLE