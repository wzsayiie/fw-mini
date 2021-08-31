#include "cuikit.h"

class MainController : public CViewController {

protected:
    void onViewLoad() override {
    }
};

static void Launch() MAPP_LAUNCH(Launch, MAppLaunchPriority_UI) {
    (new MainController)->asRootController();
}
