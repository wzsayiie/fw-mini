import { JSXElem } from './JSXObjects'
import { NView   } from './NView'

import {
    CImageView,
    CViewController,
    MImage,
} from '../host/native'

export class NImageView extends NView {
    attach(elem: JSXElem, entity: object, controller: CViewController): void {
        super.attach(elem, entity, controller)

        let imageView = <CImageView> entity

        this.getFillMode('fillMode', v => imageView.fillMode = v)
        this.getHAlign  ('hAlign'  , v => imageView.hAlign   = v)
        this.getVAlign  ('vAlign'  , v => imageView.vAlign   = v)

        this.getString('image', (v: string) => {
            if (v.startsWith('bundle://')) {
                let path  = v.substring('bundle://'.length)
                let image = MImage.fromBundle(path)
                imageView.image = image

            } else if (v.startsWith('file://')) {
                let path  = v.substring('file://'.length)
                let image = MImage.fromFile(path)
                imageView.image = image
            }
        })
    }
}
