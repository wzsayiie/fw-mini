#include "mscheduler.h"
#include <chrono>
#include "mapp.h"

define_reflectable_class_function(MScheduler, instance, "getter")
MScheduler *MScheduler::instance() {
    static MScheduler::ptr obj;
    if (!obj) {
        obj = MScheduler::create();
        MApp::instance()->addUpdateListener(0, MF<void ()>([]() {
            obj->update();
        }));
    }
    return obj.get();
}

define_reflectable_class_function(MScheduler, secondsTick, "getter")
double MScheduler::secondsTick() {
    auto at = std::chrono::system_clock::now().time_since_epoch();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(at);
    return ms.count() / 1000.0;
}

define_reflectable_class_function(MScheduler, loop, "args:seconds,task")
void MScheduler::loop(double seconds, const MFunction<void ()>::ptr &task) {
    if (seconds <= 0 || !task) {
        return;
    }

    TaskSetting setting; {
        setting.onlyOnce = false;
        setting.nextTick = secondsTick() + seconds;
        setting.interval = seconds;
    }
    mTasks.insert({ task, setting });
}

define_reflectable_class_function(MScheduler, delay, "args:seconds,task")
void MScheduler::delay(double seconds, const MFunction<void ()>::ptr &task) {
    if (!task) {
        return;
    }

    TaskSetting setting; {
        setting.onlyOnce = true;
        setting.nextTick = secondsTick() + seconds;
        setting.interval = 0;
    }
    mTasks.insert({ task, setting });
}

define_reflectable_class_function(MScheduler, cancel, "args:task")
void MScheduler::cancel(const MFunction<void ()>::ptr &task) {
    if (task) {
        mTasks.erase(task);
    }
}

void MScheduler::update() {
    std::map<MFunction<void ()>::ptr, TaskSetting> remaining;
    double now = secondsTick();

    for (auto &[task, setting] : mTasks) {

        if (setting.onlyOnce) {
            if (setting.nextTick <= now) {
                task->call();
            } else {
                remaining[task] = setting;
            }

        } else {
            if (setting.nextTick <= now) {
                task->call();
                setting.nextTick += setting.interval;
            }
            //always remained.
            remaining[task] = setting;
        }
    }

    mTasks = remaining;
}
