#include "mrunloop.h"
#include "mhostloop.h"
#include <chrono>

struct TaskConfig {
    bool  runOnlyOnce = false;
    float nextRunTick = 0;
    float interval    = 0;
    bool  cancelled   = false;
};

m_static_object(sTasks(), std::map<MLambdaRef, TaskConfig>)

float MRunningSeconds() {
    static int64_t startTick = 0;
    
    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto nowTick = std::chrono::duration_cast<std::chrono::milliseconds>(now);
    
    if (startTick == 0) {
        startTick = nowTick.count();
    }
    
    return (nowTick.count() - startTick) / 1000.f;
}

static void Update() MAPP_UPDATE(Update) {
    float tick = MRunningSeconds();

    //remove cancelled tasks.
    for (auto it = sTasks().begin(); it != sTasks().end(); ) {
        if (it->second.cancelled) {
            sTasks().erase(it++);
        } else {
            it++;
        }
    }
    
    //call tasks.
    for (auto it = sTasks().begin(); it != sTasks().end(); ++it) {
        if (tick < (it->second.nextRunTick)) {
            continue;
        }
        
        //call.
        MLambdaCall(it->first.get());
        
        //reset the task configuration.
        TaskConfig *config = &it->second;
        if (config->runOnlyOnce) {
            config->cancelled = true;
        } else {
            config->nextRunTick += config->interval;
        }
    }
}

void MRunAfterSeconds(float delay, MLambda *task) {
    if (!task) {
        return;
    }

    TaskConfig config;
    config.runOnlyOnce = true;
    config.nextRunTick = MRunningSeconds() + delay;
    config.interval    = 0;
    
    sTasks().insert({m_make_shared task, config});
}

void MRunEverySeconds(float interval, MLambda *task) {
    if (!task) {
        return;
    }

    TaskConfig config;
    config.runOnlyOnce = false;
    config.nextRunTick = MRunningSeconds() + interval;
    config.interval    = interval;
    
    sTasks().insert({m_make_shared task, config});
}

MLambdaRef MRunAfterSeconds(float delay, std::function<void ()> task) {
    MLambdaRef lambda = m_cast_lambda task;
    MRunAfterSeconds(delay, lambda.get());
    return lambda;
}

MLambdaRef MRunEverySeconds(float interval, std::function<void ()> task) {
    MLambdaRef lambda = m_cast_lambda task;
    MRunEverySeconds(interval, lambda.get());
    return lambda;
}

void MCancelTask(MLambda *task) {
    if (!task) {
        return;
    }

    auto it = sTasks().find(m_make_shared task);
    if (it != sTasks().end()) {
        it->second.cancelled = true;
    }
}
