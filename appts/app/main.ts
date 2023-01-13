import {
    CNavigationController,
    CWindow,
    MWindow,
} from "./host/native"

import {
    MainViewController,
} from "./MainViewController"

(function () {
    let mainWindow = <CWindow> MWindow.mainWindow
    let navigator  = <CNavigationController> mainWindow.rootViewController

    let page = new MainViewController()
    navigator.pushPage(page)
})()
