#include "centui.h"
#include "dash.h"
#include "minikit.h"

m_class(MainViewController, CViewController) {
protected:
    void onViewLoad() override {
    }
};

static void Launch() m_app_launch(Launch) {
    auto mainPage = MainViewController::create();

    auto navigator = CNavigationController::create();
    navigator->pushPage(mainPage);

    auto window = CWindow::create();
    window->setRootViewController(navigator);
    window->makeMainWindow();
}
