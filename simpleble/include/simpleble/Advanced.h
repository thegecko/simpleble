#pragma once

#include <simpleble/export.h>

#if __APPLE__
#include "TargetConditionals.h"
#endif

namespace SimpleBLE {

namespace Advanced {

/**
 * Advanced Features
 *
 * The functions presented in this namespace are OS-specific backdoors that are
 * not part of the standard SimpleBLE API, which allow the user to access
 * low-level details of the implementation for advanced use cases.
 *
 * These functions should be used with caution.
 */

#if defined(_WIN32)
    // Windows code

#endif

#if TARGET_OS_OSX
    // macOS code

#endif

#if TARGET_OS_IOS
    // iOS code

#endif

#if defined(__ANDROID__)
    // Android code

#endif

#if defined(__linux__) && !defined(__ANDROID__)
    // Linux code

#endif

}  // namespace Advanced

}  // namespace SimpleBLE
