#include "centui.h"

m_class(MainViewController, CViewController) {
protected:
    void onViewLoad() override {
    }
};

static void Launch() m_app_launch(Launch) {
    auto controller = MainViewController::create();
    auto mainWindow = CWindow::create();
    
    mainWindow->setRootViewController(controller);
    mainWindow->makeMainWindow();
}
