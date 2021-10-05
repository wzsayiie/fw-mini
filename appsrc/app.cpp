#include "cuikit.h"

c_class(MainController) : public CViewController {

protected:
    void onViewLoad() override {
        view()->setBackgroundColor(CColor::clearColor);
    }
};

static void Launch() MAPP_LAUNCH(Launch, MAppLaunchPriority_UI) {
    (new MainController)->asRootController();
}
