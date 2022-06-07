#include "minikit.h"

static void launch() m_app_launch(launch) {

    auto listener = MFunction<void (const std::string &)>::create([](const std::string &message) {
        dash::print(message);
    });
    MJsVM::instance()->setExceptionListener(listener);

    std::string name   = "bundle.js";
    std::string script = MFileManager::instance()->u8stringFromBundle(name);
    MJsVM::instance()->evaluate(name, script);
}
