#include "mrunloop.h"
#include "mapp.h"

struct TaskConfig {
    int  intervalTicks = 0;
    bool runOnlyOnce   = false;
    int  nextRunTick   = 0;
    bool cancelled     = false;
};

static std::map<MLambdaRef, TaskConfig> *GetTasks() {
    static auto tasks = new std::map<MLambdaRef, TaskConfig>;
    return tasks;
}

static int GetTick(int increment) {
    static int tick = 0;
    tick += increment;
    return tick;
}

MAPP_UPDATE(MAppUpdate) void MAppUpdate() {
    std::map<MLambdaRef, TaskConfig> *tasks = GetTasks();
    int tick = GetTick(1);
    
    //remove cancelled tasks.
    for (auto it = tasks->begin(); it != tasks->end(); ) {
        if (it->second.cancelled) {
            tasks->erase(it++);
        } else {
            it++;
        }
    }
    
    //call tasks.
    for (auto it = tasks->begin(); it != tasks->end(); ++it) {
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
    
    GetTasks()->insert({m_make_shared task, config});
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
    
    GetTasks()->insert({m_make_shared task, config});
}

void MCancelTask(MLambda *task) {
    if (!task) {
        return;
    }
    
    std::map<MLambdaRef, TaskConfig> *tasks = GetTasks();
    
    auto it = tasks->find(m_make_shared task);
    if (it != tasks->end()) {
        it->second.cancelled = true;
    }
}
