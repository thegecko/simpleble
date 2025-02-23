#include "ScanCallback.h"
#include <CommonUtils.h>
#include <android/log.h>
#include <fmt/format.h>

namespace SimpleBLE {
namespace Android {
namespace Bridge {

JNI::Class ScanCallback::_cls;
std::map<JNI::Object, ScanCallback*, JNI::JniObjectComparator> ScanCallback::_map;

void ScanCallback::initialize() {
    JNI::Env env;

    if (_cls.get() == nullptr) {
        _cls = env.find_class("org/simpleble/android/bridge/ScanCallback");
    }
}

ScanCallback::ScanCallback() {
    _obj = _cls.call_constructor("()V");
    _map[_obj] = this;
}

ScanCallback::~ScanCallback() { _map.erase(_obj.get()); }

void ScanCallback::set_callback_onScanResult(std::function<void(Android::ScanResult)> callback) {
    if (callback) {
        _callback_onScanResult.load(callback);
    } else {
        _callback_onScanResult.unload();
    }
}

void ScanCallback::set_callback_onBatchScanResults(std::function<void()> callback) {
    if (callback) {
        _callback_onBatchScanResults.load(callback);
    } else {
        _callback_onBatchScanResults.unload();
    }
}

void ScanCallback::set_callback_onScanFailed(std::function<void()> callback) {
    if (callback) {
        _callback_onScanFailed.load(callback);
    } else {
        _callback_onScanFailed.unload();
    }
}

void ScanCallback::jni_onScanResultCallback(JNI::Object thiz, jint callback_type, JNI::Object result) {
    auto it = ScanCallback::_map.find(thiz);
    if (it != ScanCallback::_map.end()) {
        ScanCallback* obj = it->second;
        Android::ScanResult scan_result(result);

        auto msg = fmt::format("onScanResultCallback: {}", scan_result.toString());
        __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", msg.c_str());

        SAFE_CALLBACK_CALL(obj->_callback_onScanResult, scan_result);
    } else {
        // TODO: Throw an exception
    }
}

void ScanCallback::jni_onBatchScanResultsCallback(JNI::Object thiz, JNI::Object results) {
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", "onBatchScanResultsCallback");
    // TODO: Implement
}

void ScanCallback::jni_onScanFailedCallback(JNI::Object thiz, jint error_code) {
    __android_log_write(ANDROID_LOG_INFO, "SimpleBLE", "onScanFailedCallback");
    // TODO: Implement
}

}  // namespace Bridge
}  // namespace Android
}  // namespace SimpleBLE

extern "C" {
// clang-format off
JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_ScanCallback_onScanResultCallback(JNIEnv *env, jobject thiz, jint callback_type, jobject result) {
    SimpleBLE::JNI::Object thiz_obj(thiz);
    SimpleBLE::JNI::Object result_obj(result);
    SimpleBLE::Android::Bridge::ScanCallback::jni_onScanResultCallback(thiz_obj, callback_type, result_obj);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_ScanCallback_onScanFailedCallback(JNIEnv *env, jobject thiz, jint error_code) {
    SimpleBLE::JNI::Object thiz_obj(thiz);
    SimpleBLE::Android::Bridge::ScanCallback::jni_onScanFailedCallback(thiz_obj, error_code);
}

JNIEXPORT void JNICALL Java_org_simpleble_android_bridge_ScanCallback_onBatchScanResultsCallback(JNIEnv *env, jobject thiz, jobject results) {
    SimpleBLE::JNI::Object thiz_obj(thiz);
    SimpleBLE::JNI::Object results_obj(results);
    SimpleBLE::Android::Bridge::ScanCallback::jni_onBatchScanResultsCallback(thiz_obj, results_obj);
}
// clang-format on
}