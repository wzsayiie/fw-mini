#include "cuikit.h"

def_class(MainController) : public CViewController {

protected:
    void onViewLoad() override {
        view()->setBackgroundColor(CColor::clearColor);
    }

private:
};

static void Launch() MAPP_LAUNCH(Launch, MAppLaunchPriority_UI) {
    (new MainController)->asRootController();
}
