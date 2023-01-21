import { JSXElem  } from './JSXObjects'
import { NControl } from './NControl'

import {
    CButton,
    CViewController,
} from '../host/native'

export class NButton extends NControl {
    attach(elem: JSXElem, entity: object, controller: CViewController): void {
        super.attach(elem, entity, controller)

        let button = <CButton> entity

        this.getString('title'    , v => button.title                = v)
        this.getString('text'     , v => button.titleLabel.text      = v)
        this.getColor ('textColor', v => button.titleLabel.textColor = v)
        this.getNumber('fontSize' , v => button.titleLabel.fontSize  = v)
        this.getHAlign('hAlign'   , v => button.titleLabel.hAlign    = v)
        this.getVAlign('vAlign'   , v => button.titleLabel.vAlign    = v)

        this.getString('clickTarget', (v: string) => {
            if (!v) {
                return
            }

            let func: Function = controller[v]
            if (typeof(func) != 'function') {
                return
            }

            button.clickTarget = () => {
                func.call(controller)
            }
        })
    }
}
