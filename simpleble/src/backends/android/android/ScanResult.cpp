
#include "ScanResult.h"

namespace SimpleBLE {
namespace Android {

JNI::Class ScanResult::_cls;

void ScanResult::initialize() {
    JNI::Env env;

    if (_cls.get() == nullptr) {
        _cls = env.find_class("android/bluetooth/le/ScanResult");
    }
}

ScanResult::ScanResult(jobject j_scan_result) {
    initialize();
    _obj = JNI::Object(j_scan_result, _cls.get());
};

BluetoothDevice ScanResult::getDevice() {
    return BluetoothDevice(_obj.call_object_method("getDevice", "()Landroid/bluetooth/BluetoothDevice;"));
}

int16_t ScanResult::getRssi() {
    return _obj.call_int_method("getRssi", "()I");
}

int16_t ScanResult::getTxPower() {
    return _obj.call_int_method("getTxPower", "()I");
}

bool ScanResult::isConnectable() {
    return _obj.call_boolean_method("isConnectable", "()Z");
}

ScanRecord ScanResult::getScanRecord() {
    return ScanRecord(_obj.call_object_method("getScanRecord", "()Landroid/bluetooth/le/ScanRecord;"));
}

std::string ScanResult::toString() { return _obj.call_string_method("toString", "()Ljava/lang/String;"); }

}  // namespace Android
}  // namespace SimpleBLE