#include "ScanRecord.h"

namespace SimpleBLE {
namespace Android {

JNI::Class ScanRecord::_cls;

void ScanRecord::initialize() {
    JNI::Env env;

    if (_cls.get() == nullptr) {
        _cls = env.find_class("android/bluetooth/le/ScanRecord");
    }
}

ScanRecord::ScanRecord(jobject j_scan_record) {
    initialize();
    _obj = JNI::Object(j_scan_record, _cls.get());
}

std::string ScanRecord::toString() { return _obj.call_string_method("toString", "()Ljava/lang/String;"); }

}  // namespace Android
}  // namespace SimpleBLE