import { JSXElem } from './JSXObjects'
import { NEntity } from './Nodes'

import {
    CView,
    CViewController,
    MRect,
} from '../host/native'

export class NView extends NEntity {
    attach(elem: JSXElem, controller: CViewController): void {
        super.attach(elem, controller)

        let view = <CView> this.entity
        controller.view.addSubview(view)

        this.getBoolean('interactive'    , v => view.interactive     = v)
        this.getBoolean('visible'        , v => view.visible         = v)
        this.getColor  ('backgroundColor', v => view.backgroundColor = v)
    }

    layout(x: number, y: number, w: number, h: number): void {
        (<CView> this.entity).frame = MRect.from(x, y, w, h)
    }
}
