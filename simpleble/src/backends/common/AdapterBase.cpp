#include "AdapterBase.h"

namespace SimpleBLE {

void AdapterBase::set_callback_on_power_on(std::function<void()> on_power_on) {
    if (on_power_on) {
        _callback_on_power_on.load(on_power_on);
    } else {
        _callback_on_power_on.unload();
    }
}

void AdapterBase::set_callback_on_power_off(std::function<void()> on_power_off) {
    if (on_power_off) {
        _callback_on_power_off.load(on_power_off);
    } else {
        _callback_on_power_off.unload();
    }
}

}
