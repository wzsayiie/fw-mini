#pragma once

#include "mcontainers.h"
#include "mobject.h"

m_class(MScheduler, MObject) {
public:
    static MScheduler *instance();

public:
    double secondsTick();

    void loop  (double seconds, const MFunction<void ()>::ptr &task);
    void delay (double seconds, const MFunction<void ()>::ptr &task);
    void cancel(const MFunction<void ()>::ptr &task);

private:
    void update();

private:
    struct TaskSetting {
        bool   onlyOnce = true;
        double nextTick = 0;
        double interval = 0;
    };

private:
    std::map<MFunction<void ()>::ptr, TaskSetting> mTasks;
};
