#include "mrunloop.h"
#include "mhostloop.h"
#include <chrono>

float MTickSeconds() {
    static int64_t zero = 0;

    auto now  = std::chrono::system_clock::now().time_since_epoch();
    auto tick = std::chrono::duration_cast<std::chrono::milliseconds>(now);

    if (zero == 0) {
        zero = tick.count();
    }

    return (tick.count() - zero) / 1000.f;
}

struct TaskConfig {
    bool  runOnlyOnce = false;
    float nextRunTick = 0;
    float interval    = 0;
    bool  cancelled   = false;
};
typedef std::shared_ptr<TaskConfig> TaskConfigRef;

m_static_object(sTasks(), std::map<MLambdaRef, TaskConfigRef>)

static void Update() MAPP_UPDATE(Update) {
    float tick = MTickSeconds();

    //remove cancelled tasks.
    for (auto it = sTasks().begin(); it != sTasks().end(); ) {
        if (it->second->cancelled) {
            sTasks().erase(it++);
        } else {
            it++;
        }
    }
    
    //NOTE: create a copy of "sTasks" to iterate.
    //because it to be modified possibly during the traversal.
    std::map<MLambdaRef, TaskConfigRef> tasks = sTasks();
    for (auto &pair : tasks) {
        if (tick < (pair.second->nextRunTick)) {
            continue;
        }

        MLambdaCall(pair.first.get());
        
        //reset the task configuration.
        TaskConfig *config = pair.second.get();
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

    TaskConfigRef config(new TaskConfig);
    config->runOnlyOnce = true;
    config->nextRunTick = MTickSeconds() + delay;
    
    sTasks().insert({m_make_shared task, config});
}

void MRunEverySeconds(float interval, MLambda *task) {
    if (!task) {
        return;
    }

    TaskConfigRef config(new TaskConfig);
    config->runOnlyOnce = false;
    config->nextRunTick = MTickSeconds() + interval;
    config->interval    = interval;
    
    sTasks().insert({m_make_shared task, config});
}

void MCancelTask(MLambda *task) {
    if (!task) {
        return;
    }

    auto it = sTasks().find(m_make_shared task);
    if (it != sTasks().end()) {
        it->second->cancelled = true;
    }
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
