#include "mprint.h"
#include <fstream>
#include <string>
#include "MPROFILE.h"
#include "dprint.h"
#include "mbundle.h"
#include "rdefine.h"

static const char *GetLogFilePath() {
    static std::string *path = nullptr;
    static bool init = false;

    if (init) {
        return path ? path->c_str() : nullptr;
    }

    init = true;

    const char *name = mp_log_file_name;
    if (name && *name) {
        std::string p = MBundle::instance()->getTemporaryFilePath(name);
        //NOTE: new a string.
        //prevent it from being destructed before other objects
        //at the program end.
        path = new std::string(p);
    }

    return path ? path->c_str() : nullptr;
}

static void WriteFile(const char *message) {
    const char *path = GetLogFilePath();
    if (!path) {
        return;
    }

    std::ofstream file(path, std::ios::app);
    if (file.is_open()) {
        file << message << std::endl;
    }
}

define_reflectable_function(MPrint, "args:message")
void MPrint(const char *message) {
    if (message && *message) {
        dash::print(message);
        WriteFile(message);
    }
}
