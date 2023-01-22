import { NContainer } from './Nodes'

export class NStackBox extends NContainer {
    layout(x: number, y: number, w: number, h: number): void {
        for (let child of this.children) {
            let left : number
            let width: number
            if (child.width && child.width < w) {
                left  = (w - child.width) / 2
                width = child.width
            } else {
                left  = 0
                width = w
            }

            let top   : number
            let height: number
            if (child.height && child.height < h) {
                top    = (h - child.height) / 2
                height = child.height
            } else {
                top    = 0
                height = h
            }

            child.layout(x + left, y + top, width, height)
        }
    }
}
