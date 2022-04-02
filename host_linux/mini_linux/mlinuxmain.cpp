#include <unistd.h>
#include "mapp.h"

int main() {
    MApp *app = MApp::sharedObject();

    app->launch();
    while (true) {
        usleep((useconds_t)(MApp::UpdateEverySeconds * 1000 * 1000));
        app->update();
    }
}
