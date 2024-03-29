import { JSXElem           } from './JSXObjects'
import { layoutableClasses } from './layoutableClasses'

import {
    CViewController,
    MColor,
    MFillMode,
    MHAlign,
    MVAlign,
} from '../host/native'

export class NBase {
    elem: JSXElem = null

    protected getBoolean(key: string, callback: (v: boolean) => void): void {
        let value: boolean = this.elem.props[key]
        if (typeof(value) == 'boolean') {
            callback(value)
        }
    }

    protected getNumber(key: string, callback: (v: number) => void): void {
        let value: number = this.elem.props[key]
        if (typeof(value) == 'number') {
            callback(value)
        }
    }

    protected getString(key: string, callback: (v: string) => void): void {
        let value: string = this.elem.props[key]
        if (typeof(value) == 'string') {
            callback(value)
        }
    }

    protected getObject(key: string, callback: (v: object) => void): void {
        let value: object = this.elem.props[key]
        if (typeof(value) == 'object') {
            callback(value)
        }
    }

    protected getColor(key: string, callback: (v: MColor) => void): void {
        let value: MColor = this.elem.props[key]
        if (value instanceof MColor) {
            callback(value)
        }
    }

    protected getHAlign(key: string, callback: (v: MHAlign) => void): void {
        let value: MHAlign = this.elem.props[key]
        if (typeof(value) == 'number') {
            callback(value)
        }
    }

    protected getVAlign(key: string, callback: (v: MVAlign) => void): void {
        let value: MVAlign = this.elem.props[key]
        if (typeof(value) == 'number') {
            callback(value)
        }
    }

    protected getFillMode(key: string, callback: (v: MFillMode) => void): void {
        let value: MFillMode = this.elem.props[key]
        if (typeof(value) == 'number') {
            callback(value)
        }
    }

    //need subclasses to override:

    ratio  = 1
    width  = 0
    height = 0

    attach(elem: JSXElem, _controller: CViewController): void {
        this.elem = elem

        this.getNumber('ratio' , v => this.ratio  = v)
        this.getNumber('width' , v => this.width  = v)
        this.getNumber('height', v => this.height = v)

        if (this.ratio  < 1) { this.ratio  = 1 }
        if (this.width  < 0) { this.width  = 0 }
        if (this.height < 0) { this.height = 0 }
    }

    addChild(_child: NBase): void {
    }

    layout(_x: number, _y: number, _w: number, _h: number): void {
    }
}

export class NContainer extends NBase {
    children: NBase[] = null

    addChild(child: NBase): void {
        super.addChild(child)
        
        if (!this.children) {
            this.children = []
        }
        this.children.push(child)
    }
}

export class NEntity extends NBase {
    entity = null
    name   = ''

    attach(elem: JSXElem, controller: CViewController): void {
        super.attach(elem, controller)

        let entityClass = layoutableClasses.entityClassOf(elem.type)
        if (entityClass) {
            let customObject = null
            this.getObject('customObject', v => customObject = v)

            if (customObject && customObject instanceof entityClass) {
                this.entity = customObject
            } else {
                this.entity = new entityClass()
            }
        }

        this.getString('name', v => this.name = v)
        if (this.name && this.entity) {
            controller[this.name] = this.entity
        }
    }
}
