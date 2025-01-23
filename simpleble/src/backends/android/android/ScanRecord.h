#pragma once

#include "jni/Common.hpp"

namespace SimpleBLE {
namespace Android {

class ScanRecord {
  public:
    ScanRecord(JNI::Object obj);

    std::string toString();

  private:
    static JNI::Class _cls;
    static jmethodID _method_toString;

    static void initialize();
    void check_initialized() const;
    JNI::Object _obj;
};

}  // namespace Android
}  // namespace SimpleBLE