#include "minikit.h"

static void launch() m_app_launch(launch) {

    std::string name = "bundle.js";
    std::string code = MFileManager::instance()->u8stringFromBundle(name);
    MJsVM::instance()->evaluate(name, code);
}
