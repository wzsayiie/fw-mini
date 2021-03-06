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

define_reflectable_class_function(MScheduler, GetSecondsTick)
double MScheduler::GetSecondsTick() {
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
        setting.nextTick = GetSecondsTick() + seconds;
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
        setting.nextTick = GetSecondsTick() + seconds;
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
    double now = GetSecondsTick();

    for (auto &pair : mTasks) {
        TaskSetting *setting = &pair.second;

        if (setting->onlyOnce) {
            if (setting->nextTick <= now) {
                pair.first->call();
            } else {
                remaining.insert(pair);
            }

        } else {
            if (setting->nextTick <= now) {
                pair.first->call();
                setting->nextTick += setting->interval;
            }
            //always remained.
            remaining.insert(pair);
        }
    }

    mTasks = remaining;
}
