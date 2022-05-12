#include "mscheduler.h"
#include <chrono>

define_reflectable_class_function(MScheduler, instance)
MScheduler *MScheduler::instance() {
    static auto obj = MScheduler::create();
    return obj.get();
}

define_reflectable_class_function(MScheduler, GetSecondsTick)
double MScheduler::GetSecondsTick() {
    auto at = std::chrono::system_clock::now().time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(at);
    return ms.count() / 1000.0;
}
