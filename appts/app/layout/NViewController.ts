import { JSXElem    } from './JSXObjects'
import { NResponder } from './NResponder'

import {
    CViewController,
    MRect,
} from '../host/native'

export class NViewController extends NResponder {
    attach(elem: JSXElem, entity: object, controller: CViewController): void {
        super.attach(elem, entity, controller)

        let ctrl = <CViewController> entity
        controller.addChildController(ctrl)

        this.getColor('backgroundColor', v => ctrl.view.backgroundColor = v)
    }

    layout(x: number, y: number, w: number, h: number): void {
        (<CViewController> this.entity).view.frame = MRect.from(x, y, w, h)
    }
}
