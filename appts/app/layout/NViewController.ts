import { JSXElem } from './JSXObjects'
import { NEntity } from './Nodes'

import {
    CViewController,
    MRect,
} from '../host/native'

export class NViewController extends NEntity {
    attach(elem: JSXElem, controller: CViewController): void {
        super.attach(elem, controller)

        let ctrl = <CViewController> this.entity
        controller.addChildController(ctrl)

        this.getColor('backgroundColor', v => ctrl.view.backgroundColor = v)
    }

    layout(x: number, y: number, w: number, h: number): void {
        (<CViewController> this.entity).view.frame = MRect.from(x, y, w, h)
    }
}
