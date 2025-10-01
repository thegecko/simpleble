#include "UsbHelper.h"

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/IOReturn.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/usb/IOUSBLib.h>

namespace SimpleBLE {
namespace Dongl {
namespace USB {

template <typename T, typename Deleter>
class ScopedResource {
  public:
    ScopedResource(T resource = nullptr) : resource_(resource) {}
    ~ScopedResource() {
        if (resource_) deleter_(resource_);
    }

    T get() const { return resource_; }
    T operator->() const { return resource_; }
    operator bool() const { return resource_ != nullptr; }

  private:
    T resource_;
    Deleter deleter_;
};

// Usage:
struct CFReleaser {
    void operator()(CFTypeRef obj) const { CFRelease(obj); }
};

struct IOReleaser {
    void operator()(io_object_t obj) const { IOObjectRelease(obj); }
};

using ScopedCFRef = ScopedResource<CFTypeRef, CFReleaser>;
using ScopedIOObject = ScopedResource<io_object_t, IOReleaser>;

std::vector<std::string> UsbHelper::get_dongl_devices() {
    std::vector<std::string> dongle_devices;

    // Create iterator with automatic cleanup
    io_iterator_t raw_iterator = 0;
    kern_return_t kr = IOServiceGetMatchingServices(kIOMainPortDefault, IOServiceMatching(kIOSerialBSDServiceValue),
                                                    &raw_iterator);
    if (kr != kIOReturnSuccess) return {};
    ScopedIOObject iterator(raw_iterator);

    io_service_t service;
    while ((service = IOIteratorNext(iterator.get()))) {
        // Create service with automatic cleanup
        ScopedIOObject service_guard(service);

        // Get the callout device path (/dev/cu.*) with automatic cleanup
        ScopedCFRef pathCF((CFStringRef)IORegistryEntryCreateCFProperty(service, CFSTR(kIOCalloutDeviceKey),
                                                                       kCFAllocatorDefault, 0));
        if (!pathCF) {
            continue;  // service_guard will automatically release service
        }

        // Check if this serial port matches the vendor/product by walking up the parent chain
        io_iterator_t raw_parent_iterator = 0;
        kr = IORegistryEntryCreateIterator(service, kIOServicePlane,
                                           kIORegistryIterateRecursively | kIORegistryIterateParents,
                                           &raw_parent_iterator);
        if (kr == kIOReturnSuccess) {
            ScopedIOObject parent_iterator(raw_parent_iterator);

            io_registry_entry_t parent;
            while ((parent = IOIteratorNext(parent_iterator.get()))) {
                ScopedIOObject parent_guard(parent);

                // Create CFNumber objects with automatic cleanup
                ScopedCFRef vidCF((CFNumberRef)IORegistryEntryCreateCFProperty(parent, CFSTR(kUSBVendorID),
                                                                              kCFAllocatorDefault, 0));
                ScopedCFRef pidCF((CFNumberRef)IORegistryEntryCreateCFProperty(parent, CFSTR(kUSBProductID),
                                                                              kCFAllocatorDefault, 0));

                if (vidCF && pidCF) {
                    uint16_t vid, pid;
                    CFNumberGetValue((CFNumberRef)vidCF.get(), kCFNumberSInt16Type, &vid);
                    CFNumberGetValue((CFNumberRef)pidCF.get(), kCFNumberSInt16Type, &pid);
                    if (vid == DONGL_VENDOR_ID && pid == DONGL_PRODUCT_ID) {
                        char path[PATH_MAX];
                        if (CFStringGetCString((CFStringRef)pathCF.get(), path, sizeof(path), kCFStringEncodingUTF8)) {
                            dongle_devices.push_back(std::string(path));
                        }
                    }
                }
            }
        }
    }
    // iterator automatically releases here
    return dongle_devices;
}

}  // namespace USB
}  // namespace Dongl
}  // namespace SimpleBLE
