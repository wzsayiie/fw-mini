#include "mrunloop.h"
#include "mhostloop.h"
#include <chrono>

float MTickSeconds() {
    //NOTE: the return value of "system_clock" may be very large, beyond the range of float.
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
};
typedef std::shared_ptr<TaskConfig> TaskConfigRef;

m_static_object(sTasks(), std::map<MLambdaRef, TaskConfigRef>)

static void Update() MAPP_UPDATE(Update) {
    float tick = MTickSeconds();
    
    std::map<MLambdaRef, TaskConfigRef> tasks = sTasks();
    for (auto &pair : tasks) {
        if (tick < (pair.second->nextRunTick)) {
            continue;
        }

        MLambdaCall(pair.first.get());
        
        TaskConfig *config = pair.second.get();
        if (config->runOnlyOnce) {
            sTasks().erase(pair.first);
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
    if (task) {
        sTasks().erase(m_make_shared task);
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
