#pragma once

#include "jni/Common.hpp"

namespace SimpleBLE {
namespace Android {

template <typename T>
class SparseArray {
  public:
    SparseArray();
    SparseArray(JNI::Object obj);

    int size();
    int keyAt(int index);
    T valueAt(int index);

  private:
    static JNI::Class _cls;
    static jmethodID _method_size;
    static jmethodID _method_keyAt;
    static jmethodID _method_valueAt;

    static void initialize();
    void check_initialized() const;
    JNI::Object _obj;
};

}  // namespace Android
}  // namespace SimpleBLE
