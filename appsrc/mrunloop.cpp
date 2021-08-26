#include "mrunloop.h"
#include "mhostloop.h"

struct TaskConfig {
    int  intervalTicks = 0;
    bool runOnlyOnce   = false;
    int  nextRunTick   = 0;
    bool cancelled     = false;
};

m_static_object(sTasks(), std::map<MLambdaRef, TaskConfig>)

static int GetTick(int increment) {
    static int tick = 0;
    tick += increment;
    return tick;
}

static void Update() MAPP_UPDATE(Update) {
    int tick = GetTick(1);
    
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
            config->nextRunTick += config->intervalTicks;
        }
    }
}

void MRunAfterSeconds(float delay, MLambda *task) {
    if (!task) {
        return;
    }
    
    auto intervalTicks = (int)(delay / _MAppUpdateInterval);
    
    TaskConfig config;
    config.intervalTicks = intervalTicks;
    config.runOnlyOnce   = true;
    config.nextRunTick   = GetTick(0) + intervalTicks;
    
    sTasks().insert({m_make_shared task, config});
}

void MRunEverySeconds(float interval, MLambda *task) {
    if (!task) {
        return;
    }
    
    auto intervalTicks = (int)(interval / _MAppUpdateInterval);
    
    TaskConfig config;
    config.intervalTicks = intervalTicks;
    config.runOnlyOnce   = false;
    config.nextRunTick   = GetTick(0) + intervalTicks;
    
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
