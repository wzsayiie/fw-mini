import { CViewController } from '../host/native'
import { JSXElem         } from './JSXObjects'

export class BaseNode {
    elem: JSXElem = null

    protected getBoolean(key: string, callback: (v: boolean) => void): void {
        let str = this.elem.props[key]
        if (str === undefined) {
            return
        }
        
        let bol = Boolean(str)
        callback(bol)
    }

    protected getNumber(key: string, callback: (v: number) => void): void {
        let str = this.elem.props[key]
        if (str === undefined) {
            return
        }
        
        let num = Number(str)
        callback(!isNaN(num) ? num : 0)
    }

    protected getString(key: string, callback: (v: string) => void): void {
        let str = this.elem.props[key]
        if (str === undefined) {
            return
        }
        
        callback(str)
    }

    //need subclasses to override:

    ratio  = 1
    width  = 0
    height = 0

    attach(elem: JSXElem, _entity: object, _controller: CViewController): void {
        this.elem = elem

        this.getNumber('ratio' , v => this.ratio  = v)
        this.getNumber('width' , v => this.width  = v)
        this.getNumber('height', v => this.height = v)

        if (this.ratio  < 1) { this.ratio  = 1 }
        if (this.width  < 0) { this.width  = 0 }
        if (this.height < 0) { this.height = 0 }
    }

    addChild(_child: BaseNode): void {
    }

    layout(_x: number, _y: number, _w: number, _h: number): void {
    }
}

export class ContainerNode extends BaseNode {
    children: BaseNode[] = null

    addChild(child: BaseNode): void {
        super.addChild(child)
        
        if (!this.children) {
            this.children = []
        }
        this.children.push(child)
    }
}

export class EntityNode extends BaseNode {
    name   = ''
    entity = null

    attach(elem: JSXElem, entity: object, controller: CViewController): void {
        super.attach(elem, entity, controller)

        this.getString('name', v => this.name = v)
        this.entity = entity

        if (this.name && entity) {
            controller[this.name] = entity
        }
    }
}
