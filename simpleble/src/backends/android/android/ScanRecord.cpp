#include "ScanRecord.h"

namespace SimpleBLE {
namespace Android {

JNI::Class ScanRecord::_cls;
jmethodID ScanRecord::_method_toString = nullptr;

void ScanRecord::initialize() {
    JNI::Env env;

    if (_cls.get() == nullptr) {
        _cls = env.find_class("android/bluetooth/le/ScanRecord");
    }

    if (!_method_toString) {
        _method_toString = env->GetMethodID(_cls.get(), "toString", "()Ljava/lang/String;");
    }
}

ScanRecord::ScanRecord(JNI::Object obj) : _obj(obj) { initialize(); }

void ScanRecord::check_initialized() const {
    if (!_obj) throw std::runtime_error("ScanRecord is not initialized");
}

std::string ScanRecord::toString() {
    check_initialized();
    return _obj.call_string_method(_method_toString);
}

}  // namespace Android
}  // namespace SimpleBLE