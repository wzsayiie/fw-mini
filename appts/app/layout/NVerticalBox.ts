import { NLinearBox } from './NLinearBox'

export class NVerticalBox extends NLinearBox {
    layout(x: number, y: number, w: number, h: number): void {
        let ranges = this.calcLayout(h, 'height')

        for (let i = 0; i < ranges.length; ++i) {
            let child = this.children[i]
            let range = ranges[i]

            let left : number
            let width: number
            if (child.width && child.width < w) {
                left  = (w - child.width) / 2
                width = child.width
            } else {
                left  = 0
                width = w
            }

            child.layout(x + left, y + range.loc, width, range.len)
        }
    }
}
