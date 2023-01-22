import { LayoutableController } from "./layout/LayoutableController"
import { layout } from './MainViewLayout'

import {
    CButton,
    CView,
} from "./host/native"

export class MainViewController extends LayoutableController {

    onViewLoad(): void {
        this.setLayout(layout)
    }
}
