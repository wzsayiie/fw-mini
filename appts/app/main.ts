import {
    CNavigationController,
    CViewController,
    CWindow,
    MWindow,
} from "./host/native"

class MainViewController extends CViewController {

    onViewLoad(): void {
    }
}

(function () {
    let mainWindow = <CWindow> MWindow.mainWindow
    let navigator  = <CNavigationController> mainWindow.rootViewController

    let page = new MainViewController()
    navigator.pushPage(page)
})()
