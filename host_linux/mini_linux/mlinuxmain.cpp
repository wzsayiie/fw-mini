#include <unistd.h>
#include "mapp.h"
#include "mpcbundle.h"

int main() {
    MPCBundle::install();

    MApp *app = MApp::instance();
    app->launch();

    auto interval = (useconds_t)(MApp::UpdateEverySeconds * 1000 * 1000);
    while (true) {
        usleep(interval);
        app->update();
    }
}
