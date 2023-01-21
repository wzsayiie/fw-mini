import { JSXElem } from './JSXObjects'
import { NView   } from './NView'

import {
    CScrollView,
    CViewController,
} from '../host/native'

export class NScrollView extends NView {
    attach(elem: JSXElem, entity: object, controller: CViewController): void {
        super.attach(elem, entity, controller)

        let scrollView = <CScrollView> entity

        let contentWidth  = 0
        let contentHeight = 0
        this.getNumber('contentWidth' , v => contentWidth  = v)
        this.getNumber('contentHeight', v => contentHeight = v)

        if (contentWidth > 0 && contentHeight > 0) {
            scrollView.setContentSize(contentWidth, contentHeight)
        }
    }
}
