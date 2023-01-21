import { JSXElem  } from './JSXObjects'
import { NControl } from './NControl'

import {
    CTextField,
    CViewController,
} from '../host/native'

export class NTextField extends NControl {
    attach(elem: JSXElem, entity: object, controller: CViewController): void {
        super.attach(elem, entity, controller)

        let textField = <CTextField> entity

        this.getString('prompt'   , v => textField.prompt    = v)
        this.getColor ('textColor', v => textField.textColor = v)
        this.getNumber('fontSize' , v => textField.fontSize  = v)
        this.getHAlign('hAlign'   , v => textField.hAlign    = v)
        this.getVAlign('vAlign'   , v => textField.vAlign    = v)
    }
}
