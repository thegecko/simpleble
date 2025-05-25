#include <simpleble/Utils.h>
#if defined(SIMPLEBLE_BACKEND_MACOS)
#include "TargetConditionals.h"
#endif

namespace SimpleBLE {

OperatingSystem get_operating_system() {
#ifdef _WIN32
    return OperatingSystem::WINDOWS;
#elif defined(TARGET_OS_OSX)
    return OperatingSystem::MACOS;
#elif defined(TARGET_OS_IOS)
    return OperatingSystem::IOS;
#elif __linux__
    return OperatingSystem::LINUX;
#elif __ANDROID__
    return OperatingSystem::ANDROID;
#endif
}

std::string get_simpleble_version() { return SIMPLEBLE_VERSION; }

}  // namespace SimpleBLE