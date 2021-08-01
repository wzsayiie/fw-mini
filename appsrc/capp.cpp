#include "cuikit.h"

class MainController : public CViewController {

protected:
    void onViewLoad() override {
    }
};

static void Launch() MAPP_UI_LAUNCH(Launch) {
    (new MainController)->asRootController();
}
