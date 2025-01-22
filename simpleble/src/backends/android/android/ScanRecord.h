#pragma once

#include "jni/Common.hpp"

namespace SimpleBLE {
namespace Android {

class ScanRecord {
  public:
    ScanRecord(jobject j_scan_record);

    std::string toString();

  private:
    static JNI::Class _cls;
    static void initialize();

    JNI::Object _obj;
};

}  // namespace Android
}  // namespace SimpleBLE