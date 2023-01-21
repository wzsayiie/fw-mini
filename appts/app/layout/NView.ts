import { JSXElem    } from './JSXObjects'
import { NResponder } from './NResponder'

import {
    CView,
    CViewController,
    MRect,
} from '../host/native'

export class NView extends NResponder {
    attach(elem: JSXElem, entity: object, controller: CViewController): void {
        super.attach(elem, entity, controller)

        let view = <CView> entity
        controller.view.addSubview(view)

        this.getBoolean('interactive'    , v => view.interactive     = v)
        this.getBoolean('visible'        , v => view.visible         = v)
        this.getColor  ('backgroundColor', v => view.backgroundColor = v)
    }

    layout(x: number, y: number, w: number, h: number): void {
        (<CView> this.entity).frame = MRect.from(x, y, w, h)
    }
}
