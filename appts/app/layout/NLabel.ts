import { JSXElem } from './JSXObjects'
import { NView   } from './NView'

import {
    CLabel,
    CViewController,
} from '../host/native'

export class NLabel extends NView {
    attach(elem: JSXElem, entity: object, controller: CViewController): void {
        super.attach(elem, entity, controller)

        let label = <CLabel> entity

        this.getString('text'     , v => label.text      = v)
        this.getColor ('textColor', v => label.textColor = v)
        this.getNumber('fontSize' , v => label.fontSize  = v)
        this.getHAlign('hAlign'   , v => label.hAlign    = v)
        this.getVAlign('vAlign'   , v => label.vAlign    = v)
    }
}
