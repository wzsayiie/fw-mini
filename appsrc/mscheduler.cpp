#include "mscheduler.h"
#include <chrono>

MScheduler *MScheduler::sharedObject() {
    static auto obj = MScheduler::create();
    return obj.get();
}

double MScheduler::GetSecondsTick() {
    auto at = std::chrono::system_clock::now().time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(at);
    return ms.count() / 1000.0;
}
