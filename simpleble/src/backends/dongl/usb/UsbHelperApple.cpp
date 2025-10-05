#include "UsbHelperApple.h"

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

struct CFReleaser {
    void operator()(CFTypeRef obj) const { CFRelease(obj); }
};

struct IOReleaser {
    void operator()(io_object_t obj) const { IOObjectRelease(obj); }
};

using ScopedCFRef = ScopedResource<CFTypeRef, CFReleaser>;
using ScopedIOObject = ScopedResource<io_object_t, IOReleaser>;

UsbHelperApple::UsbHelperApple(const std::string& device_path) : UsbHelperImpl(device_path) {}

UsbHelperApple::~UsbHelperApple() = default;

void UsbHelperApple::tx(const kvn::bytearray& data) {
    // Apple-specific implementation
}

void UsbHelperApple::set_rx_callback(std::function<void(const kvn::bytearray&)> callback) {
    _rx_callback.load(callback);
}

std::vector<std::string> UsbHelperApple::get_dongl_devices() {
    std::vector<std::string> dongle_devices;

    io_iterator_t raw_iterator = 0;
    if (IOServiceGetMatchingServices(kIOMainPortDefault, IOServiceMatching(kIOSerialBSDServiceValue), &raw_iterator) !=
        kIOReturnSuccess) {
        return {};
    }
    ScopedIOObject iterator(raw_iterator);

    io_service_t raw_service;
    while ((raw_service = IOIteratorNext(iterator.get()))) {
        ScopedIOObject service_guard(raw_service);

        // Get the callout device raw_path (/dev/cu.*)
        char raw_path[PATH_MAX];
        ScopedCFRef pathCF((CFStringRef)IORegistryEntryCreateCFProperty(raw_service, CFSTR(kIOCalloutDeviceKey),
                                                                        kCFAllocatorDefault, 0));
        if (!pathCF) {
            continue;
        }

        if (!CFStringGetCString((CFStringRef)pathCF.get(), raw_path, sizeof(raw_path), kCFStringEncodingUTF8)) {
            continue;
        }

        std::string path(raw_path);

        // Search up parents for idVendor and idProduct
        ScopedCFRef vidCF((CFNumberRef)IORegistryEntrySearchCFProperty(
            raw_service, kIOServicePlane, CFSTR(kUSBVendorID), kCFAllocatorDefault,
            kIORegistryIterateParents | kIORegistryIterateRecursively));
        ScopedCFRef pidCF((CFNumberRef)IORegistryEntrySearchCFProperty(
            raw_service, kIOServicePlane, CFSTR(kUSBProductID), kCFAllocatorDefault,
            kIORegistryIterateParents | kIORegistryIterateRecursively));

        if (!vidCF || !pidCF) {
            continue;
        }

        uint16_t vid, pid;
        CFNumberGetValue((CFNumberRef)vidCF.get(), kCFNumberSInt16Type, &vid);
        CFNumberGetValue((CFNumberRef)pidCF.get(), kCFNumberSInt16Type, &pid);

        if (vid == UsbHelperImpl::DONGL_VENDOR_ID && pid == UsbHelperImpl::DONGL_PRODUCT_ID) {
            dongle_devices.push_back(path);
        }
    }
    return dongle_devices;
}

}  // namespace USB
}  // namespace Dongl
}  // namespace SimpleBLE
