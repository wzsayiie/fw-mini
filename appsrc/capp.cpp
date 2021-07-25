#include "cuikit.h"

class MainController : public CViewController {

protected:
    void onViewLoad() override {
    }
};

MAPP_UI_LAUNCH(CAppLaunch) void CAppLaunch() {
    (new MainController)->asRootController();
}
