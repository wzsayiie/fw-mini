import { JSXElem } from './JSXObjects'
import { NView   } from './NView'

import {
    CControl,
    CViewController,
} from '../host/native'

export class NControl extends NView {
    attach(elem: JSXElem, entity: object, controller: CViewController): void {
        super.attach(elem, entity, controller)

        let control = <CControl> entity

        this.getString('controlId'      , v => control.controlId       = v)
        this.getString('previousControl', v => control.previousControl = v)
        this.getString('nextControl'    , v => control.nextControl     = v)
    }
}
