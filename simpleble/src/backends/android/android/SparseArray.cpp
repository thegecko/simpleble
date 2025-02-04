#include "SparseArray.h"

namespace SimpleBLE {
namespace Android {

template <typename T>
JNI::Class SparseArray<T>::_cls;
template <typename T>
jmethodID SparseArray<T>::_method_size = nullptr;
template <typename T>
jmethodID SparseArray<T>::_method_keyAt = nullptr;
template <typename T>
jmethodID SparseArray<T>::_method_valueAt = nullptr;

template <typename T>
void SparseArray<T>::initialize() {
    JNI::Env env;

    if (_cls.get() == nullptr) {
        _cls = env.find_class("android/util/SparseArray");
    }

    if (!_method_size) {
        _method_size = env->GetMethodID(_cls.get(), "size", "()I");
    }

    if (!_method_keyAt) {
        _method_keyAt = env->GetMethodID(_cls.get(), "keyAt", "(I)I");
    }

    if (!_method_valueAt) {
        _method_valueAt = env->GetMethodID(_cls.get(), "valueAt", "(I)Ljava/lang/Object;");
    }
}

template <typename T>
SparseArray<T>::SparseArray() {
    initialize();
}

template <typename T>
SparseArray<T>::SparseArray(JNI::Object obj) : _obj(obj) {
    initialize();
}

template <typename T>
void SparseArray<T>::check_initialized() const {
    if (!_obj) throw std::runtime_error("SparseArray is not initialized");
}

template <typename T>
int SparseArray<T>::size() {
    check_initialized();
    return _obj.call_int_method(_method_size);
}

template <typename T>
int SparseArray<T>::keyAt(int index) {
    check_initialized();
    return _obj.call_int_method(_method_keyAt, index);
}

template <typename T>
T SparseArray<T>::valueAt(int index) {
    check_initialized();
    return T(_obj.call_object_method(_method_valueAt, index));
}

template class SparseArray<JNI::ByteArray>;

}  // namespace Android
}  // namespace SimpleBLE