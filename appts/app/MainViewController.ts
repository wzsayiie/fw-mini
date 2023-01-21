import { LayoutableController } from "./layout/LayoutableController"
import { layout } from './MainViewLayout'

import {
    CButton,
} from "./host/native"

export class MainViewController extends LayoutableController {

    private _button: CButton

    onViewLoad(): void {
        this.setLayout(layout)
    }
}
